#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <algorithm>

#include "../data.h"
#include "../tree_utils.h"
#include "bst.h"

namespace color {
    constexpr const char* reset   = "\033[0m"; //cuztomizable
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
        std::cerr << "Usage: ./bst <search|stats> <n_docs> <directory>\n";
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
        std::cerr << "Error: <n_docs> must be a positive integer.\n";
        return 1;
    }

    std::cout << color::bold << "Command: " << color::reset << command
              << ",  Documents: "      << n_docs_int
              << ",  Directory: "      << dir_path << "\n";

    TREE::BinaryTree* bst_tree = TREE::createTree();
    TREE::AggregateStats bst_stats;
    bst_stats.tree_type = "BST";

    const auto overall_indexing_start_time = std::chrono::high_resolution_clock::now();

    std::vector<std::string> all_filenames = DATA::list_files_txt_in_path(dir_path);
    if (all_filenames.empty()) {
        std::cerr << "Error: No .txt files found in '" << dir_path << "'.\n";
        return 1;
    }

    const int files_to_process_count =
        std::min(static_cast<int>(all_filenames.size()), n_docs_int);

    bst_stats.num_docs_indexed = files_to_process_count;
    std::cout << "Processing the first " << files_to_process_count << " docs.\n";

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
                      << "'. Using -1.\n";
        }

        const std::vector<std::string> words_in_doc =
            DATA::tokenize(full_path_to_file);

        if (words_in_doc.empty()) {
            std::cerr << "Warning: no tokens in file: " << full_path_to_file << '\n';
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
              << bst_stats.total_indexing_time_ms << " ms.\n";

    bst_stats.final_node_count  = TREE::countNodes(bst_tree->root);
    bst_stats.final_tree_height = TREE::calculateHeight(bst_tree->root);

    if (command == "stats") {
        using std::setw;
        std::cout << color::bold << "\n■ Index statistics\n" << color::reset
                  << setw(22) << "Docs indexed : " << bst_stats.num_docs_indexed        << '\n'
                  << setw(22) << "Unique words  : " << bst_stats.final_node_count         << '\n'
                  << setw(22) << "Tree height   : " << bst_stats.final_tree_height        << '\n'
                  << setw(22) << "Words proc.   : " << bst_stats.total_words_processed    << '\n'
                  << setw(22) << "Comparisons   : " << bst_stats.total_comparisons_insertion << '\n';

        TREE::save_stats_to_csv(bst_stats);
    }

    else if (command == "search") {
        std::string input_line;

        while (true) {
            std::cout << color::bold
                      << "\nInsert words separated by spaces "
                      << "(or '/q' to quit):\n> "
                      << color::reset;

            if (!std::getline(std::cin >> std::ws, input_line)) break; // EOF (^D)

            if (input_line == "/q") break;
            if (input_line.empty()) continue;

            std::istringstream iss(input_line);
            std::vector<std::string> queries;
            for (std::string raw; iss >> raw; ) {
                std::string norm = DATA::normalise(raw);
                if (!norm.empty()) queries.push_back(norm);
            }
            if (queries.empty()) {
                std::cout << color::yellow << "No valid tokens. Try again.\n"
                          << color::reset;
                continue;
            }

            for (const auto& q : queries) {
                TREE::SearchResult r = TREE::search(bst_tree, q);

                std::cout << '\n'
                          << color::cyan << color::bold << q << color::reset << ":\n";

                std::cout << "    Found       : "
                          << (r.found ? (std::string{color::green}  + "yes")
                                      : (std::string{color::red}    + "no "))
                          << color::reset << "\n";

                std::cout << "    Docs        : ";
                if (r.documentIds.empty()) {
                    std::cout << "-";
                } else {
                    for (size_t i = 0; i < r.documentIds.size(); ++i) {
                        if (i) std::cout << ' ';
                        std::cout << r.documentIds[i];
                    }
                }
                std::cout << "\n";

                std::cout << "    Comparisons : "
                          << color::yellow << r.numComparisons << color::reset << "\n";

                std::cout << "    Time (ms)   : "
                          << color::yellow << std::fixed << std::setprecision(3)
                          << r.executionTime << color::reset << '\n';
            }
        }
    }

    else {
        std::cerr << "Error: unknown command '" << command
                  << "'. Use 'search' or 'stats'.\n";
        TREE::destroy(bst_tree);
        return 1;
    }

    std::cout << "\nDeallocating tree…\n";
    TREE::destroy(bst_tree);
    return 0;
}
