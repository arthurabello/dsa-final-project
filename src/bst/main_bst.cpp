#include <iostream>
#include <string>

#include "bst.h"
#include "../cli.h"
#include "../tree_utils.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: ./bst <search|stats|view> <n_docs> <directory>" << endl;
        return 1;
    }

    const string command     { argv[1] };
    const string numFilesStr { argv[2] };
    const string directory   { argv[3] };

    int numFiles = CLI::validateNumFiles(numFilesStr);

    cout << CLI::colorize(CLI::color::bold, "Command: ") << command << ", "
        << CLI::colorize(CLI::color::bold, "Documents: ") << numFiles << ", "
        << CLI::colorize(CLI::color::bold, "Directory: ") << directory << endl;

    TREE::BinaryTree* bst_tree = TREE::createTree();

    int error;
    auto stats = CLI::indexFilesInDir(
        directory, numFiles, &error,
        [&](string word, int documentId) {
            return TREE::BST::insert(bst_tree, word, documentId);
        }
    );
    if (error) return 1;

    stats.final_node_count  = TREE::countNodes(bst_tree->root);
    stats.final_tree_height = TREE::calculateHeight(bst_tree->root);
    stats.tree_type = "BST";

    if (command == "stats") {
        CLI::printAndExportStats(stats);
    }
    else if (command == "search") {
        CLI::searchFiles(bst_tree);
    }
    else if (command == "view") {
        CLI::startViewServer(bst_tree);
    }
    else {
        cerr << "Error: Unkowned command: " << command << "."
                  << "Use 'search', 'view' or 'stats'." << endl;

        TREE::destroy(bst_tree);
        return 1;
    }

    cout << endl << "Deallocating tree…" << endl;

    TREE::destroy(bst_tree);
    return 0;
}
