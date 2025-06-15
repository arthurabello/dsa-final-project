#include "rbt.h"          
#include "tree_utils.h"   
#include "../../Unity/src/unity.h"

void setUp(void) {}

void tearDown(void) {}

namespace TREE::RBT{

    // Helper functions to assert some rbt properties =====================
    // should i put this here or in rbt.cpp? or in tree_utils?

    // Verifies is a node is black or null (black also)
    bool is_black(TREE::Node* node) {


    }

    // Verifies is there isn't consecutive red nodes to mantain accuracy
    void verify_no_consecutive_red_nodes(TREE::Node* node) {


    }

    // Verifies if all paths from node to leaf have the same black height
    // Returns black height or -1 if there is any violation 
    int verify_black_height(TREE::Node* node) {


    }



    // REAL TESTS ======================================================

    // test insertion of a single node. root must be always black
    void test_single_node_insertion_is_black() {
        BinaryTree* tree = createTree();
        
        deleteBinaryTree(tree);
    }

    // inserts that cause simple recoloring
    void test_recoloring_case() {
        BinaryTree* tree = createTree();
        //
        deleteBinaryTree(tree);
    }

    // simple rotation to the right (Left-Left Case)
    void test_right_rotation_case() {
        //
        deleteBinaryTree(tree);
    }

    // simple rotation to the left (Right-Right Case)
    void test_left_rotation_case() {
        BinaryTree* tree = createTree();
        //
        deleteBinaryTree(tree);
    }

    // insertion of the same word in different docs
    void test_same_word_different_docs_insertion() {
        BinaryTree* tree = createTree();
        //
        deleteBinaryTree(tree);
    }


    void test_complex_insertion_and_search() {
        BinaryTree* tree = createTree();

        // multiple rotations and inserts

        deleteBinaryTree(tree);
    }
}


int main() {
    UNITY_BEGIN();
    // teeeests
    return UNITY_END();
}


