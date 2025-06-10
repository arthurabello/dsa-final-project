#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm>

#include "../data.h"   
#include "../tree_utils.h"
#include "bst.h"

#include "../../tinyhttp/http.cpp"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./bst <search|stats> <n_docs> <directory>\n";
        return 1;
    }

    const std::string command   { argv[1] };
    const std::string n_docs_str{ argv[2] };
    const std::string dir_path  { argv[3] };

    int n_docs_int = 0;
    try {
        n_docs_int = std::stoi(n_docs_str);
        if (n_docs_int <= 0) throw std::invalid_argument("non‑positive");
    } catch (...) {
        std::cerr << "Error: <n_docs> must be a positive integer.\n";
        return 1;
    }

    std::cout << "Command: " << command
              << ", Number of documents: " << n_docs_int
              << ", Directory: " << dir_path << "\n";

    TREE::BinaryTree* bst_tree = TREE::createTree();
    TREE::AggregateStats bst_stats;
    bst_stats.tree_type = "BST";

    const auto overall_indexing_start_time = std::chrono::high_resolution_clock::now();

    std::vector<std::string> all_filenames = DATA::list_files_txt_in_path(dir_path);
    if (all_filenames.empty()) {
        std::cerr << "Error: No .txt files found in '" << dir_path << "'.\n";
        return 1;
    }

    const int files_to_process_count = std::min(static_cast<int>(all_filenames.size()), n_docs_int);
    bst_stats.num_docs_indexed = files_to_process_count;
    std::cout << "Processing the first " << files_to_process_count << " docs.\n";

    for (int i = 0; i < files_to_process_count; ++i) { //index loop
        const std::string& filename_only = all_filenames[i];
        std::string full_path_to_file = dir_path;
        if (!dir_path.empty() && dir_path.back() != '/' && dir_path.back() != '\\') {
            full_path_to_file += '/';
        }
        full_path_to_file += filename_only;

        int document_id = -1;
        try {
            const size_t dot_pos = filename_only.find('.');
            if (dot_pos != std::string::npos)
                document_id = std::stoi(filename_only.substr(0, dot_pos));
        } catch (...) {
            std::cerr << "Warning: cannot parse doc‑id for '" << filename_only << "'. Using -1.\n";
        }

        const std::vector<std::string> words_in_doc = DATA::tokenize(full_path_to_file); //tokenize and insert
        if (words_in_doc.empty()) {
            std::cerr << "Warning: no tokens in file: " << full_path_to_file << '\n';
            continue;
        }

        for (const std::string& word : words_in_doc) {
            if (word.empty()) continue;
            bst_stats.total_words_processed++;
            TREE::InsertResult res = TREE::BST::insert(bst_tree, word, document_id);
            bst_stats.sum_of_insertion_times_ms   += res.executionTime;
            bst_stats.total_comparisons_insertion += res.numComparisons;
            bst_stats.max_insertion_time_ms = std::max(bst_stats.max_insertion_time_ms, res.executionTime);
        }
    }

    const auto overall_indexing_end_time = std::chrono::high_resolution_clock::now(); //finish indexing and record time
    bst_stats.total_indexing_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        overall_indexing_end_time - overall_indexing_start_time).count();

    std::cout << "Indexing completed in " << bst_stats.total_indexing_time_ms << " ms.\n";

    bst_stats.final_node_count  = TREE::countNodes(bst_tree->root); //tree structural stats
    bst_stats.final_tree_height = TREE::calculateHeight(bst_tree->root);

    if (command == "stats") {
        std::cout << "Documents indexed : " << bst_stats.num_docs_indexed        << '\n'
                  << "Unique words      : " << bst_stats.final_node_count         << '\n'
                  << "Tree height       : " << bst_stats.final_tree_height        << '\n'
                  << "Total words proc. : " << bst_stats.total_words_processed    << '\n'
                  << "Total comparisons : " << bst_stats.total_comparisons_insertion << '\n';

        TREE::save_stats_to_csv(bst_stats);

    } else if (command == "search") {
        std::string input_line;
        bool header_printed = false;

        while (true) {
            std::cout << "Insert words separated by spaces (or 'exit_search' to quit):\n> ";
            if (!std::getline(std::cin >> std::ws, input_line)) break;   // EOF ^D
            if (input_line == "exit_search") break;
            if (input_line.empty()) continue;

            std::istringstream iss(input_line); //normalize and collect queries
            std::vector<std::string> queries;
            for (std::string raw; iss >> raw; ) {
                std::string norm = DATA::normalise(raw);
                if (!norm.empty()) queries.push_back(norm);
            }
            if (queries.empty()) {
                std::cout << "No valid tokens. Try again.\n";
                continue;
            }

            struct Row { std::string word; TREE::SearchResult res; }; //run each query and display uga buga csv table
            std::vector<Row> rows;
            rows.reserve(queries.size());
            for (const auto& q : queries) rows.push_back({ q, TREE::search(bst_tree, q) });

            if (!header_printed) {
                std::cout << "word,found,docs,comparisons,time_ms\n";
                header_printed = true;
            }

            for (const auto& r : rows) {
                std::cout << r.word << ',' << r.res.found << ",\"";
                for (size_t i = 0; i < r.res.documentIds.size(); ++i) {
                    if (i) std::cout << ' ';
                    std::cout << r.res.documentIds[i];
                }
                std::cout << "\"," << r.res.numComparisons << ',' << r.res.executionTime << '\n';
            }
        }
    } else if (command == "view") {
        // static é necessário pra acessar a variável de dentro do `requested`
        static std::string json = "[";

        std::function<void(TREE::Node*)> add_node = [&](TREE::Node *n) {
            if (n == nullptr) { return; }

            size_t id = (size_t) n; // Usa posição na memória como ID
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
        std::cerr << "Error: Unkowned command: " << command << ". Use 'search' or 'stats'." << std::endl;
        TREE::destroy(bst_tree);
        return 1;
    }

    std::cout << "Deallocating tree...\n"; //cleanup
    TREE::destroy(bst_tree);
    return 0;
}
