#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm>

#include "../data.h"   
#include "../tree_utils.h"
#include "bst.h"

#include "../../tinyhttp/http.cpp"

namespace color {
    constexpr const char* reset   = "\033[0m";
    constexpr const char* bold    = "\033[1m";
    constexpr const char* red     = "\033[31m";
    constexpr const char* green   = "\033[32m";
    constexpr const char* yellow  = "\033[33m";
    constexpr const char* blue    = "\033[34m";
    constexpr const char* magenta = "\033[35m";
    constexpr const char* cyan    = "\033[36m";
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./bst <search|stats> <n_docs> <directory>" << std::endl;
        return 1;
    }

    const std::string command   { argv[1] };
    const std::string n_docs_str{ argv[2] };
    const std::string dir_path  { argv[3] };

    int n_docs_int = 0;
    try {
        n_docs_int = std::stoi(n_docs_str);
        if (n_docs_int <= 0) throw std::invalid_argument("non-positive");
    } catch (...) {
        std::cerr << "Error: <n_docs> must be a positive integer." << std::endl;
        return 1;
    }

    std::cout << color::bold << "Command: " << color::reset << command
              << ",  Documents: "      << n_docs_int
              << ",  Directory: "      << dir_path << std::endl;

    TREE::BinaryTree* bst_tree = TREE::createTree();
    TREE::AggregateStats bst_stats;
    bst_stats.tree_type = "BST";

    const auto overall_indexing_start_time = std::chrono::high_resolution_clock::now();

    std::vector<std::string> all_filenames = DATA::list_files_txt_in_path(dir_path);
    if (all_filenames.empty()) {
        std::cerr << "Error: No .txt files found in '" << dir_path << std::endl;
        return 1;
    }

    const int files_to_process_count =
        std::min(static_cast<int>(all_filenames.size()), n_docs_int);

    bst_stats.num_docs_indexed = files_to_process_count;
    std::cout << "Processing the first " << files_to_process_count << " docs." << std::endl;

    for (int i = 0; i < files_to_process_count; ++i) { //index loop
        const std::string& filename_only = all_filenames[i];
        std::string full_path_to_file = dir_path;
        if (!dir_path.empty() && dir_path.back() != '/' && dir_path.back() != '\\')
            full_path_to_file += '/';
        full_path_to_file += filename_only;

        int document_id = -1;
        try {
            const size_t dot_pos = filename_only.find('.');
            if (dot_pos != std::string::npos)
                document_id = std::stoi(filename_only.substr(0, dot_pos));
        } catch (...) {
            std::cerr << "Warning: cannot parse doc-id for '" << filename_only
                      << "'. Using -1." << std::endl;
        }

        const std::vector<std::string> words_in_doc =
            DATA::tokenize(full_path_to_file);

        if (words_in_doc.empty()) {
            std::cerr << "Warning: no tokens in file: " << full_path_to_file << std::endl;
            continue;
        }

        for (const std::string& word : words_in_doc) {
            if (word.empty()) continue;
            bst_stats.total_words_processed++;

            TREE::InsertResult res =
                TREE::BST::insert(bst_tree, word, document_id);

            bst_stats.sum_of_insertion_times_ms   += res.executionTime;
            bst_stats.total_comparisons_insertion += res.numComparisons;
            bst_stats.max_insertion_time_ms =
                std::max(bst_stats.max_insertion_time_ms, res.executionTime);
        }
    }

    const auto overall_indexing_end_time = std::chrono::high_resolution_clock::now();
    bst_stats.total_indexing_time_ms = std::chrono::duration_cast
        <std::chrono::milliseconds>(overall_indexing_end_time -
                                    overall_indexing_start_time).count();

    std::cout << "Indexing completed in "
              << bst_stats.total_indexing_time_ms << " ms." << std::endl;

    bst_stats.final_node_count  = TREE::countNodes(bst_tree->root);
    bst_stats.final_tree_height = TREE::calculateHeight(bst_tree->root);

    if (command == "stats") {
        using std::setw;
        std::cout << color::bold << std::endl << " Index statistics" << std::endl << color::reset
                  << setw(22) << "Docs indexed : " << bst_stats.num_docs_indexed        << std::endl;
                  << setw(22) << "Unique words  : " << bst_stats.final_node_count         << std::endl;
                  << setw(22) << "Tree height   : " << bst_stats.final_tree_height        << std::endl;
                  << setw(22) << "Words proc.   : " << bst_stats.total_words_processed    << std::endl;
                  << setw(22) << "Comparisons   : " << bst_stats.total_comparisons_insertion << std::endl;

        TREE::save_stats_to_csv(bst_stats);
    }

    else if (command == "search") {
        std::string input_line;

        while (true) {
            std::cout << color::bold
                      << std::endl << "Insert words separated by spaces "
                      << "(or '/q' to quit): " << std::endl
                      << color::reset;

            if (!std::getline(std::cin >> std::ws, input_line)) break; 

            if (input_line == "/q") break;
            if (input_line.empty()) continue;

            std::istringstream iss(input_line);
            std::vector<std::string> queries;
            for (std::string raw; iss >> raw; ) {
                std::string norm = DATA::normalise(raw);
                if (!norm.empty()) queries.push_back(norm);
            }
            if (queries.empty()) {
                std::cout << color::yellow << "No valid tokens. Try again." << std::endl
                          << color::reset;
                continue;
            }

            for (const auto& q : queries) {
                TREE::SearchResult r = TREE::search(bst_tree, q);

                std::cout << std::endl
                          << color::cyan << color::bold << q << color::reset << ":" << std::endl;

                std::cout << "    Found       : "
                          << (r.found ? (std::string{color::green}  + "yes")
                                      : (std::string{color::red}    + "no "))
                          << color::reset << std::endl;

                std::cout << "    Docs        : ";
                if (r.documentIds.empty()) {
                    std::cout << "-";
                } else {
                    for (size_t i = 0; i < r.documentIds.size(); ++i) {
                        if (i) std::cout << ' ';
                        std::cout << r.documentIds[i];
                    }
                }
                std::cout << std::endl;

                std::cout << "    Comparisons : "
                          << color::yellow << r.numComparisons << color::reset << std::endl;

                std::cout << "    Time (ms)   : "
                          << color::yellow << std::fixed << std::setprecision(3)
                          << r.executionTime << color::reset << std::endl;
            }
        }
    } else if (command == "view") {
        // static is needed to acess var inside `requested`
        static std::string json = "[";

        std::function<void(TREE::Node*)> add_node = [&](TREE::Node *n) {
            if (n == nullptr) { return; }

            size_t id = (size_t) n; 
            size_t parent_id = (size_t) n->parent;
            json += "[\"" + n->word + "\",";
            json += std::to_string(id) + ",";
            json += std::to_string(parent_id) + ",";
            json += std::to_string(n->documentIds.size()) + "],";

            add_node(n->left);
            add_node(n->right);
        };
        add_node(bst_tree->root);
        
        json.pop_back();
        json += "]";

        HttpServer server;
        server.when("/")->serveFile("view/index.html");
        server.whenMatching("/static/[^/]+")->serveFromFolder("view");
        server.when("/data")->requested([](const HttpRequest& req) {
            return HttpResponse { 200, "text/plain", json };
        });
        std::cout << "Live on: http://127.0.0.1:8055" << std::endl;
        server.startListening(8055);

    } else {
        std::cerr << "Error: Unkowned command: " << command
                  << ". Use 'search' or 'stats'." << std::endl;
        TREE::destroy(bst_tree);
        return 1;
    }

    std::cout << "\nDeallocating tree…" << std::endl;
    TREE::destroy(bst_tree);
    return 0;
}
