#include "../tree_utils.cpp"
#include <chrono>

using namespace std::chrono;

namespace TREE::AVL {
    InsertResult insert(BinaryTree& binary_tree, const std::string& word, int documentId) {
        InsertResult result;
        int comparisons = 0;
        auto start_time = high_resolution_clock::now();

        Node& newNode = createNode(word, {documentId});

        if(binary_tree.root == nullptr) {
            binary_tree.root = &newNode;
        } else {
            Node* current = binary_tree.root;
            
            while (current != nullptr) {
                if (word < current->word) {
                    current = current->left;
                    continue;
                }
            }
        }

        return result;
    }
}
