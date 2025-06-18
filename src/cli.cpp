// This module generalizes common functionality between main_bst, main_avl and
// main_rbt

#include <chrono>
#include <climits>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "cli.h"
#include "tree_utils.h"
#include "../tinyhttp/http.hpp"
#include "data.h"

using namespace std;
using namespace chrono;

namespace CLI {
    string formatDouble(double num, int decimalPlaces) {
        string str = to_string(num);
        int la = str.find_last_of('.');
        return str.substr(0, la + decimalPlaces + 1);
    }
    
    string colorize(const char* c, string str) {
        return c + str + color::reset;
    }
    
    void startViewServer(TREE::BinaryTree* tree, VisualizationStats stats) {
        // 'static' is necessary to access the variable from inside 'requested'
        static string json = "{\"nodes\": [";
    
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
        json += "],";

        // Insertion times
        json += "\"insertTimes\": [";
        for (float time : stats.insertTimes) {
            json += to_string((int) time) + ",";
        }
        json.pop_back();
        json += "],";

        // Search times
        json += "\"searchTimes\": [";
        for (float time : stats.searchTimes) {
            json += to_string((int) time) + ",";
        }
        json.pop_back();
        json += "],";

        // Number of comparisons
        json += "\"numComparisons\": [";
        for (int num : stats.numComparisons) {
            json += to_string(num) + ",";
        }
        json.pop_back();
        json += "]}";
    
        HttpServer server;

        // '/'            => returns 'tree.html'
        server.when("/")->serveFile("view/tree.html");
    
        // '/page'        => returns 'page.html'
        server.when("/tree")->serveFile("view/tree.html");
        server.when("/freq")->serveFile("view/freq.html");
        server.when("/search")->serveFile("view/search.html");
        server.when("/insert")->serveFile("view/insert.html");
        server.when("/comparisons")->serveFile("view/comparisons.html");

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

    int indexFilesInDir(
        const string& directory,
        int numFiles,
        int *docsIndexed,
        function<TREE::InsertResult(string, int)> callback
    ) {
        auto startTime = chrono::high_resolution_clock::now();

        vector<string> filenames = DATA::listTxtFilesInDirectory(directory, numFiles);
        if (filenames.empty()) {
            cerr << "Error: No .txt files found in '" << directory << "'." << endl;
            return 1;
        }

        const int numFilesToProcess = min(filenames.size(), (unsigned long) numFiles);
        *docsIndexed = numFilesToProcess;

        cout << "Indexing the first " << numFilesToProcess << " docs..." << endl;

        for (int i = 0; i < numFilesToProcess; i++) {
            const string& filename = filenames[i];

            // Guaranteed from the implementation
            int documentId = i + 1;

            vector<string> words = DATA::tokenize(filename);

            if (words.empty()) {
                cerr << "Warning: no tokens in file: " << filename << endl;
                continue;
            }

            for (const string& word : words) {
                // Calls a tree specific (BST/AVL/RBT) function to deal with
                // the words found
                callback(word, documentId);
            }
        }

        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

        cout << "Indexing completed in "
            << duration.count() << " ms." << endl;

        return 0;
    }

    void testSearch(TREE::BinaryTree *tree, VisualizationStats *stats, string directory) {
        cout << "Collecting search statistics..." << endl;

        // For now, let's use the words from one of the last files to be the
        // least biased we can
        vector<string> words = DATA::tokenize(directory + "/" + "10000.txt");

        TREE::SearchResult res;   // Used to get "numComparisons"
        const int numTries = 100; // How many times to search for each word

        for (string word : words) {
            auto startTime = high_resolution_clock().now();

            for (int i = 0; i < numTries; i++) {
                res = TREE::search(tree, word);
            }

            auto endTime = high_resolution_clock().now();
            auto duration = duration_cast<nanoseconds>(endTime - startTime);
            auto placeholder = (endTime - startTime);
            stats->searchTimes.push_back(((float) placeholder.count()) / numTries);
            stats->numComparisons.push_back(res.numComparisons);
        }

        cout << "Done." << endl;
    }

    TREE::AggregateStats collectAggStats(TREE::BinaryTree *tree, VisualizationStats *stats) {
        TREE::AggregateStats aggStats;

        aggStats.maxComparisonsSearch = 0;
        aggStats.minComparisonsSearch = INT_MAX;
        aggStats.totalComparisonsSearch = 0;

        for (auto &e : stats->numComparisons) {
            aggStats.totalComparisonsSearch += e;

            if (e > aggStats.maxComparisonsSearch) {
                aggStats.maxComparisonsSearch = e;
            }
            if (e < aggStats.minComparisonsSearch) {
                aggStats.minComparisonsSearch = e;
            }
        }
        aggStats.avgComparisonsSearch = ((float) aggStats.totalComparisonsSearch) / stats->numComparisons.size();

        aggStats.maxSearchTimeNs = 0;
        aggStats.minSearchTimeNs = INT64_MAX;
        aggStats.totalSearchTimeNs = 0;

        for (auto &e : stats->searchTimes) {
            aggStats.totalSearchTimeNs += e;

            if (e > aggStats.maxSearchTimeNs) {
                aggStats.maxSearchTimeNs = e;
            }
            if (e < aggStats.minSearchTimeNs) {
                aggStats.minSearchTimeNs = e;
            }
        }
        aggStats.avgSearchTimeNs = aggStats.totalSearchTimeNs / stats->searchTimes.size();

        aggStats.maxInsertionTimeNs = 0;
        aggStats.minInsertionTimeNs = INT64_MAX;
        aggStats.totalInsertionTimeNs = 0;

        for (auto &e : stats->insertTimes) {
            aggStats.totalInsertionTimeNs += e;

            if (e > aggStats.maxInsertionTimeNs) {
                aggStats.maxInsertionTimeNs = e;
            }
            if (e < aggStats.minInsertionTimeNs) {
                aggStats.minInsertionTimeNs = e;
            }
        }
        aggStats.avgInsertionTimeNs = aggStats.totalInsertionTimeNs / stats->insertTimes.size();

        aggStats.treeHeight = TREE::calculateHeight(tree->root);
        aggStats.treeMinDepth = TREE::calculateMinDepth(tree->root);
        aggStats.relativeBalance = (float) aggStats.treeHeight / aggStats.treeMinDepth;
        aggStats.balanceDiff = aggStats.treeHeight - aggStats.treeMinDepth;
        aggStats.nodeCount = TREE::countNodes(tree->root);
        aggStats.docsIndexed = stats->docsIndexed;
        aggStats.wordsIndexed = stats->wordsIndexed;

        return aggStats;
    }

    void saveAsCsv(TREE::AggregateStats stats, string filename) {
        ofstream file(filename);

        if (!file.is_open()) {
            cerr << "Error: Unable to open file '"
                << filename << "' for writing." << endl;

            return;
        }

        // Header
        file
            << "docsIndexed,"
            << "wordsIndexed,"

            << "totalInsertionTimeNs,"
            << "avgInsertionTimeNs,"
            << "maxInsertionTimeNs,"
            << "minInsertionTimeNs,"

            << "totalSearchTimeNs,"
            << "avgSearchTimeNs,"
            << "maxSearchTimeNs,"
            << "minSearchTimeNs,"

            << "totalComparisonsSearch,"
            << "avgComparisonsSearch,"
            << "maxComparisonsSearch,"
            << "minComparisonsSearch,"

            << "nodeCount,"
            << "treeHeight,"
            << "treeMinDepth,"
            << "balanceDiff,"
            << "relativeBalance";

        file << endl;

        // Data
        file
            << stats.docsIndexed << ","
            << stats.wordsIndexed << ","
            << stats.totalInsertionTimeNs << ","
            << stats.avgInsertionTimeNs << ","
            << stats.maxInsertionTimeNs << ","
            << stats.minInsertionTimeNs << ","
            << stats.totalSearchTimeNs << ","
            << stats.avgSearchTimeNs << ","
            << stats.maxSearchTimeNs << ","
            << stats.minSearchTimeNs << ","
            << stats.totalComparisonsSearch << ","
            << stats.avgComparisonsSearch << ","
            << stats.maxComparisonsSearch << ","
            << stats.minComparisonsSearch << ","
            << stats.nodeCount << ","
            << stats.treeHeight << ","
            << stats.treeMinDepth << ","
            << stats.balanceDiff << ","
            << stats.relativeBalance;

        file << endl;

        file.close();
        std::cout << "Statistics saved to: " << filename << std::endl;
    }
}
