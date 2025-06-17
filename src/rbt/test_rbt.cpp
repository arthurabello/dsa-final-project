#include "rbt.h"          
#include "../tree_utils.h" 
#include "../../Unity/src/unity.h"

void setUp(void) {}

void tearDown(void) {}

namespace TREE::RBT{

    // Helper functions to assert some rbt properties =====================
    // should i put this here or in rbt.cpp? or in tree_utils?

    // Verifies is a node is black or null (black also)
    bool is_black(TREE::Node* node) {         
        return node == nullptr || node->isRed == 0;
    }

    // Verifies is there isn't consecutive red nodes to mantain accuracy
    void verify_no_consecutive_red_nodes(TREE::Node* node) {
        if (node == nullptr) return;

        if (node->isRed) {
            // If current node is red its children must be black
            TEST_ASSERT_TRUE_MESSAGE(is_black(node->left), "Consecutive Red Nodes found (left child).");
            TEST_ASSERT_TRUE_MESSAGE(is_black(node->right), "Consecutive Red Nodes found (right child).");
        }

        verify_no_consecutive_red_nodes(node->left);
        verify_no_consecutive_red_nodes(node->right);
    }

    // Verifies if all paths from node to leaf have the same black height
    // Returns black height or -1 if there is any violation 
    int verify_black_height(TREE::Node* node) {
        if (node == nullptr) {
            return 1; // path for null leafs have black height 1 
        }

        int left_black_height = verify_black_height(node->left);
        int right_black_height = verify_black_height(node->right);

        // If any subtree already violated property, propagate error 
        if (left_black_height == -1 || right_black_height == -1) {
            return -1;
        }

        // Violation for different black heights
        if (left_black_height != right_black_height) {
            TEST_FAIL_MESSAGE("Black height property violated.");
            return -1;
        }

        // Black height for this path is the black height from children +1 if this node is black
        return left_black_height + (is_black(node) ? 1 : 0); // ternary operator ;P
    }

    // Calls all verifications
    void verify_rbt_properties(TREE::BinaryTree* tree) {
        if (tree == nullptr || tree->root == nullptr) {
            TEST_PASS(); // null tree is a valid rbt     I THINK (?)
            return;
        }

        TEST_ASSERT_EQUAL_INT_MESSAGE(0, tree->root->isRed, "VIOLATION: Root node is not black.");

        verify_no_consecutive_red_nodes(tree->root);

        TEST_ASSERT_TRUE_MESSAGE(verify_black_height(tree->root) != -1, "Black height property failed.");
    }



    // REAL TESTS ======================================================

    // Simple insertion =====

    void test_insert_single_word() {
        BinaryTree* tree = createTree();
        insert(tree, "banana", 1);
        
        TEST_ASSERT_NOT_NULL(tree->root);
        TEST_ASSERT_EQUAL_STRING("banana", tree->root->word.c_str());
        verify_rbt_properties(tree);
        
        TREE::destroy(tree);
    }

    // insertion of the same word in different docs
    void test_insert_same_word_multiple_docs() {
        BinaryTree* tree = createTree();
        insert(tree, "figo", 10);
        insert(tree, "figo", 20);
        
        TEST_ASSERT_EQUAL_INT(2, tree->root->documentIds.size());
        TEST_ASSERT_EQUAL_INT(10, tree->root->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(20, tree->root->documentIds[1]);
        TEST_ASSERT_NULL(tree->root->left); 
        TEST_ASSERT_NULL(tree->root->right);
        verify_rbt_properties(tree);
        
        TREE::destroy(tree);
    }


    // Rotations and recoloring =====

    void test_recoloring_with_red_uncle() {
        BinaryTree* tree = createTree();
        insert(tree, "manga", 1);    // Root (black)
        insert(tree, "goiaba", 2);  // Left child (red)
        insert(tree, "uva", 3);      // Right child (red)
    
        TEST_ASSERT_EQUAL_STRING("manga", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_INT(1, tree->root->left->isRed);
        TEST_ASSERT_EQUAL_INT(1, tree->root->right->isRed);
        
        // Inserting "damasco" forces a recoloring because the uncle "uva" is red.
        insert(tree, "damasco", 4);
        
        // The root ("manga") must remain black.
        // The parent ("goiaba") and uncle ("uva") should be recolored to black.
        // The grandparent ("manga") would try to become red, but as it's the root, it remains black.
        TEST_ASSERT_EQUAL_INT(0, tree->root->isRed);
        TEST_ASSERT_EQUAL_INT(0, tree->root->left->isRed); // goiaba (black)
        TEST_ASSERT_EQUAL_INT(0, tree->root->right->isRed); // uva (black)
        TEST_ASSERT_EQUAL_INT(1, tree->root->left->left->isRed); // damasco (red)
        verify_rbt_properties(tree);

        TREE::destroy(tree);
    }

    void test_case_right_right_causes_left_rotation() {
        BinaryTree* tree = createTree();
        insert(tree, "damasco", 1);
        insert(tree, "goiaba", 2);
        insert(tree, "manga", 3); // This causes a left rotation at the root "damasco"
        
        // Expected structure: goiaba(B) is the new root, with damasco(R) and manga(R) as children.
        TEST_ASSERT_EQUAL_STRING("goiaba", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_STRING("damasco", tree->root->left->word.c_str());
        TEST_ASSERT_EQUAL_STRING("manga", tree->root->right->word.c_str());
        verify_rbt_properties(tree);
        
        TREE::destroy(tree);
    }

    void test_case_left_left_causes_right_rotation() {
        BinaryTree* tree = createTree();
        insert(tree, "zebra", 1);
        insert(tree, "macaco", 2);
        insert(tree, "baleia", 3); // This causes a right rotation at the root "zebra"
        
        // Expected structure: macaco(B) is the new root, with zebra(R) and baleia(R) as children.
        TEST_ASSERT_EQUAL_STRING("macaco", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_STRING("baleia", tree->root->left->word.c_str());
        TEST_ASSERT_EQUAL_STRING("zebra", tree->root->right->word.c_str());
        verify_rbt_properties(tree);

        TREE::destroy(tree);
    }

    void test_case_left_right_causes_double_rotation() {
        BinaryTree* tree = createTree();
        insert(tree, "xicara", 1);
        insert(tree, "colher", 2);
        insert(tree, "garfo", 3); // This causes a double rotation: left on "colher", then right on "xicara"

        // Expected structure: garfo(B) is the new root, with colher(R) and xicara(R) as children.
        TEST_ASSERT_EQUAL_STRING("garfo", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_STRING("colher", tree->root->left->word.c_str());
        TEST_ASSERT_EQUAL_STRING("xicara", tree->root->right->word.c_str());
        verify_rbt_properties(tree);

        TREE::destroy(tree);
    }

    void test_case_right_left_causes_double_rotation() {
        BinaryTree* tree = createTree();
        insert(tree, "razao", 1);
        insert(tree, "zenite", 2);
        insert(tree, "sonho", 3); // This causes a double rotation: right on "zenite", then left on "razao"

        // Expected structure: sonho(B) is the new root, with razao(R) and zenite(R) as children.
        TEST_ASSERT_EQUAL_STRING("sonho", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_STRING("razao", tree->root->left->word.c_str());
        TEST_ASSERT_EQUAL_STRING("zenite", tree->root->right->word.c_str());
        verify_rbt_properties(tree);

        TREE::destroy(tree);
    }

    // Search Function =====

    void test_search_on_rbt() {
        BinaryTree* tree = createTree();
        insert(tree, "laranja", 10);
        insert(tree, "banana", 20);
        insert(tree, "uva", 30);
        insert(tree, "abacaxi", 40);
        insert(tree, "figo", 50);
        insert(tree, "morango", 60);
        
        // Verifies that the final tree is a valid RBT before testing the search
        verify_rbt_properties(tree);

        // Tests search for a node that is the root (after balancing)
        SearchResult r1 = search(tree, "laranja");
        TEST_ASSERT_TRUE(r1.found);
        TEST_ASSERT_EQUAL_INT(10, r1.documentIds[0]);

        // Tests search for a node that is a leaf
        SearchResult r2 = search(tree, "abacaxi");
        TEST_ASSERT_TRUE(r2.found);
        TEST_ASSERT_EQUAL_INT(40, r2.documentIds[0]);
        
        // Tests search for an internal node
        SearchResult r3 = search(tree, "uva");
        TEST_ASSERT_TRUE(r3.found);
        TEST_ASSERT_EQUAL_INT(30, r3.documentIds[0]);

        // Tests search for a word that does not exist
        SearchResult r4 = search(tree, "melancia");
        TEST_ASSERT_FALSE(r4.found);

        // Tests search on an empty tree
        BinaryTree* empty_tree = createTree();
        SearchResult r5 = search(empty_tree, "qualquercoisa");
        TEST_ASSERT_FALSE(r5.found);
        
        destroy(tree);
        destroy(empty_tree);
    }
}


int main() {
    UNITY_BEGIN();
    
    RUN_TEST(TREE::RBT::test_insert_single_word);
    RUN_TEST(TREE::RBT::test_insert_same_word_multiple_docs);
    RUN_TEST(TREE::RBT::test_recoloring_with_red_uncle);
    RUN_TEST(TREE::RBT::test_case_right_right_causes_left_rotation);
    RUN_TEST(TREE::RBT::test_case_left_left_causes_right_rotation);
    RUN_TEST(TREE::RBT::test_case_left_right_causes_double_rotation);
    RUN_TEST(TREE::RBT::test_case_right_left_causes_double_rotation);
    RUN_TEST(TREE::RBT::test_search_on_rbt);
    
    return UNITY_END();
}


