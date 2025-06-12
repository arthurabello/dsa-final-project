// This module generalizes common functionality between main_bst, main_avl and
// main_rbt

#include <functional> // Used for additional functionality
#include <iostream>
#include <string>

#include "cli.h"
#include "tree_utils.h"
#include "../tinyhttp/http.cpp"
#include "data.h"

using namespace std;

namespace CLI {
    string formatDouble(double num, int decimalPlaces) {
        string str = to_string(num);
        int la = str.find_last_of('.');
        return str.substr(0, la + decimalPlaces + 1);
    }
    
    string colorize(const char* c, string str) {
        return c + str + color::reset;
    }
    
    void startViewServer(TREE::BinaryTree* tree) {
        // 'static' is necessary to access the variable from inside 'requested'
        static string json = "[";
    
        // The block below creates a JSON string with all of the tree nodes
        // represented as [word, ID, parent ID, number of file occurrences] to be
        // loaded with Javascript
        function<void(TREE::Node*)> add_node = [&](TREE::Node *n) {
            if (n == nullptr) { return; }
    
            size_t id = (size_t) n; // Usa posição na memória como ID :P
            size_t parent_id = (size_t) n->parent;
            json += "[\"" + n->word + "\",";
            json += to_string(id) + ",";
            json += to_string(parent_id) + ",";
            json += to_string(n->documentIds.size()) + "],";
    
            add_node(n->left);
            add_node(n->right);
        };
    
        // Starts writing the JSON recursively
        add_node(tree->root);
        
        // Removes the last ','
        json.pop_back();
        json += "]";
    
        HttpServer server;
    
        // '/'            => returns 'index.html'
        server.when("/")->serveFile("view/index.html");
    
        // '/static/file' => returns the files' content
        server.whenMatching("/static/[^/]+")->serveFromFolder("view");
    
        // '/data'        => returns the JSON created above
        server.when("/data")->requested([](const HttpRequest& _req) {
            return HttpResponse { 200, "text/plain", json };
        });
    
        cout << endl << "Live on:" <<
            colorize(color::green, " http://127.0.0.1:8055") << endl;
    
        // Start the server
        server.startListening(8055);
    }
    
    void searchFiles(TREE::BinaryTree* tree) {
        string input_line;
    
        while (true) {
            cout << color::bold
                      << "\nInsert words separated by spaces "
                      << "(or '/q' to quit):\n> "
                      << color::reset;
    
            if (!getline(cin >> ws, input_line)) break; // EOF (^D)
    
            if (input_line == "/q") break;
            if (input_line.empty()) continue;
    
            istringstream iss(input_line);
            vector<string> queries;
            for (string raw; iss >> raw; ) {
                string norm = DATA::normalise(raw);
                if (!norm.empty()) queries.push_back(norm);
            }
            if (queries.empty()) {
                cout << color::yellow << "No valid tokens. Try again.\n"
                          << color::reset;
                continue;
            }
    
            for (const auto& q : queries) {
                TREE::SearchResult r = TREE::search(tree, q);
    
                cout << '\n'
                    << color::bold << colorize(color::cyan, q) << ":" << endl;
    
                cout << "    Found       : "
                    << (r.found ? colorize(color::green, "yes")
                                : colorize(color::red, "no "))
                    << endl;
    
                cout << "    Docs        : ";
    
                if (r.documentIds.empty()) {
                    cout << "-";
                } else {
                    for (size_t i = 0; i < r.documentIds.size(); ++i) {
                        if (i) cout << ' ';
                        cout << r.documentIds[i];
                    }
                }
    
                cout << endl;
    
                cout << "    Comparisons : "
                    << colorize(color::yellow, to_string(r.numComparisons))
                    << endl;
    
                cout << "    Time (ms)   : "
                    << colorize(color::yellow, formatDouble(r.executionTime, 3))
                    << endl;
            }
        }
    }

    void printAndExportStats(TREE::AggregateStats stats) {
        cout << endl << colorize(color::bold, "■ Index statistics") << endl
          << "Docs indexed : " << stats.num_docs_indexed            << endl
          << "Unique words : " << stats.final_node_count            << endl
          << "Tree height  : " << stats.final_tree_height           << endl
          << "Words proc.  : " << stats.total_words_processed       << endl
          << "Comparisons  : " << stats.total_comparisons_insertion << endl;

        TREE::save_stats_to_csv(stats);
    }

    int validateNumFiles(string arg) {
        int n = -1;
        try {
            n = stoi(arg);
            if (n <= 0) throw invalid_argument("non-positive");

        } catch (...) { // Catch any parsing error
            cerr << "Error: <n_docs> must be a positive integer.\n";
        }

        return n;
    }

    TREE::AggregateStats indexFilesInDir(
        const string& directory,
        int numFiles,
        int *error,
        function<TREE::InsertResult(string, int)> callback
    ) {
        *error = 0;

        TREE::AggregateStats stats;

        const auto indexingStart = chrono::high_resolution_clock::now();

        vector<string> filenames = DATA::list_txt_files_in_path(directory);
        if (filenames.empty()) {
            cerr << "Error: No .txt files found in '" << directory << "'." << endl;
            *error = 1;
            return stats;
        }

        const int numFilesToProcess =
            min(filenames.size(), (unsigned long) numFiles);

        stats.num_docs_indexed = numFilesToProcess;
        cout << "Processing the first " << numFilesToProcess << " docs." << endl;

        for (int i = 0; i < numFilesToProcess; ++i) {
            const string& filename = filenames[i];

            // Get the document ID from the filename
            int documentId = -1;
            try {
                const size_t dotPos = filename.find('.');
                const size_t slashPos = filename.find_last_of('/');
                documentId = stoi(filename.substr(slashPos + 1, dotPos));
            } catch (...) {
                cerr << "Warning: cannot parse doc-id for '" << filename
                    << "'. Using -1." << endl;
            }

            const vector<string> words_in_doc = DATA::tokenize(filename);

            if (words_in_doc.empty()) {
                cerr << "Warning: no tokens in file: " << filename << endl;
                continue;
            }

            for (const string& word : words_in_doc) {
                if (word.empty()) continue;
                stats.total_words_processed++;

                // Calls a tree specific (BST/AVL/RBT) function to deal with
                // the words found
                TREE::InsertResult res = callback(word, documentId);

                stats.sum_of_insertion_times_ms   += res.executionTime;
                stats.total_comparisons_insertion += res.numComparisons;
                stats.max_insertion_time_ms = max(
                    stats.max_insertion_time_ms,
                    res.executionTime
                );
            }
        }

        const auto overall_indexing_end_time = chrono::high_resolution_clock::now();
        stats.total_indexing_time_ms =
            chrono::duration_cast<chrono::milliseconds>(
                overall_indexing_end_time - indexingStart
            ).count();

        cout << "Indexing completed in "
            << stats.total_indexing_time_ms << " ms." << endl;

        return stats;
    }
}
