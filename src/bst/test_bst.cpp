#include "bst.h"
#include "../../Unity/src/unity.h"

void setUp(void) {}

void tearDown(void) {}

namespace TREE::BST {
    void test_one_insertion() {
        BinaryTree* tree = createTree();
        InsertResult result = insert(tree, "arvore", 1);
    
        TEST_ASSERT_NOT_NULL(tree->root);
		
        TEST_ASSERT_EQUAL_STRING((const char*) "arvore", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds.size());
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(0, result.numComparisons);
    }
    
    void test_left_tree_insertion() {
        BinaryTree* tree = createTree();
    
        InsertResult r1 = insert(tree, "zebra", 1);
        InsertResult r2 = insert(tree, "yak", 2);
        InsertResult r3 = insert(tree, "xexeu", 3);
    
        TEST_ASSERT_EQUAL_STRING("zebra", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_STRING("yak", tree->root->left->word.c_str());
        TEST_ASSERT_EQUAL_STRING("xexeu", tree->root->left->left->word.c_str());
    
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(2, tree->root->left->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(3, tree->root->left->left->documentIds[0]);
    
        TEST_ASSERT_EQUAL_INT(0, r1.numComparisons);
        TEST_ASSERT_EQUAL_INT(1, r2.numComparisons);
        TEST_ASSERT_EQUAL_INT(2, r3.numComparisons);
    }
    
    void test_right_tree_insertion() {
        BinaryTree* tree = createTree();
    
        InsertResult r1 = insert(tree, "arara", 1);
        InsertResult r2 = insert(tree, "baleia", 2);
        InsertResult r3 = insert(tree, "cachorro", 3);
    
        TEST_ASSERT_EQUAL_STRING("arara", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_STRING("baleia", tree->root->right->word.c_str());
        TEST_ASSERT_EQUAL_STRING("cachorro", tree->root->right->right->word.c_str());
    
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(2, tree->root->right->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(3, tree->root->right->right->documentIds[0]);
    
        TEST_ASSERT_EQUAL_INT(0, r1.numComparisons);
        TEST_ASSERT_EQUAL_INT(1, r2.numComparisons);
        TEST_ASSERT_EQUAL_INT(2, r3.numComparisons);
    }
    
    void test_generic_tree_insertion() {
        BinaryTree* tree = createTree();
    
        InsertResult r1 = insert(tree, "macaco", 1);
        InsertResult r2 = insert(tree, "girafa", 2);
        InsertResult r3 = insert(tree, "tigre", 3);
        InsertResult r4 = insert(tree, "jabuti", 4);
        InsertResult r5 = insert(tree, "panda", 5);
    
        TEST_ASSERT_EQUAL_STRING("macaco", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_STRING("girafa", tree->root->left->word.c_str());
        TEST_ASSERT_EQUAL_STRING("jabuti", tree->root->left->right->word.c_str());
        TEST_ASSERT_EQUAL_STRING("tigre", tree->root->right->word.c_str());
        TEST_ASSERT_EQUAL_STRING("panda", tree->root->right->left->word.c_str());
    
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(2, tree->root->left->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(4, tree->root->left->right->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(3, tree->root->right->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(5, tree->root->right->left->documentIds[0]);
    
        TEST_ASSERT_EQUAL_INT(0, r1.numComparisons);
        TEST_ASSERT_EQUAL_INT(1, r2.numComparisons);
        TEST_ASSERT_EQUAL_INT(1, r3.numComparisons);
        TEST_ASSERT_EQUAL_INT(2, r4.numComparisons);
        TEST_ASSERT_EQUAL_INT(2, r5.numComparisons);
    }
    
    void test_numbers_and_words_tree_insertion() {
        BinaryTree* tree = createTree();
    
        InsertResult r1 = insert(tree, "12345", 1);
        InsertResult r2 = insert(tree, "sucuri", 2);
        InsertResult r3 = insert(tree, "23450", 3);
        InsertResult r4 = insert(tree, "ornitorrinco", 4);
    
        TEST_ASSERT_EQUAL_STRING("12345", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_STRING("sucuri", tree->root->right->word.c_str());
        TEST_ASSERT_EQUAL_STRING("23450", tree->root->right->left->word.c_str());
        TEST_ASSERT_EQUAL_STRING("ornitorrinco", tree->root->right->left->right->word.c_str());

        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(2, tree->root->right->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(3, tree->root->right->left->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(4, tree->root->right->left->right->documentIds[0]);

        TEST_ASSERT_EQUAL_INT(0, r1.numComparisons);
        TEST_ASSERT_EQUAL_INT(1, r2.numComparisons);  
        TEST_ASSERT_EQUAL_INT(2, r3.numComparisons);  
        TEST_ASSERT_EQUAL_INT(3, r4.numComparisons);  

    }
    
    void test_different_words_same_doc_tree_insertion(){
        BinaryTree* tree = createTree();
        InsertResult r1 = insert(tree, "cobra", 1);
        InsertResult r2 = insert(tree, "jararaca", 1);
    
        TEST_ASSERT_EQUAL_STRING("cobra", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_STRING("jararaca", tree->root->right->word.c_str());
    
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(1, tree->root->right->documentIds[0]);
    
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds.size());
        TEST_ASSERT_EQUAL_INT(1, tree->root->right->documentIds.size());
    
        TEST_ASSERT_EQUAL_INT(0, r1.numComparisons);
        TEST_ASSERT_EQUAL_INT(1, r2.numComparisons);
    }
    
    void test_same_word_same_doc_tree_insertion(){
        BinaryTree* tree = createTree();
        InsertResult r1 = insert(tree, "capivara", 1);
        InsertResult r2 = insert(tree, "capivara", 1);
    
        TEST_ASSERT_EQUAL_STRING("capivara", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds.size());
        TEST_ASSERT_NULL(tree->root->left);
        TEST_ASSERT_NULL(tree->root->right);
    
        TEST_ASSERT_EQUAL_INT(0, r1.numComparisons);
        TEST_ASSERT_EQUAL_INT(1, r2.numComparisons);
    }
    
    void test_same_word_different_docs_insertion(){
        BinaryTree* tree = createTree();
        InsertResult r1 = insert(tree, "tucano", 1);
        InsertResult r2 = insert(tree, "tucano", 2);
    
        TEST_ASSERT_EQUAL_STRING("tucano", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(2, tree->root->documentIds[1]);
        TEST_ASSERT_EQUAL_INT(2, tree->root->documentIds.size());
        TEST_ASSERT_NULL(tree->root->left);
        TEST_ASSERT_NULL(tree->root->right);
    
        TEST_ASSERT_EQUAL_INT(0, r1.numComparisons);
        TEST_ASSERT_EQUAL_INT(1, r2.numComparisons);
    }
    
    void test_similar_words_insertion() {
        BinaryTree* tree = createTree();
    
        InsertResult r1 = insert(tree, "e", 1);
        InsertResult r2 = insert(tree, "el", 2);
        InsertResult r3 = insert(tree, "eli", 3);
        InsertResult r4 = insert(tree, "elia", 4);
        InsertResult r5 = insert(tree, "elian", 4);
        InsertResult r6 = insert(tree, "eliane", 3);
    
        TEST_ASSERT_EQUAL_STRING("e", tree->root->word.c_str());
        TEST_ASSERT_EQUAL_STRING("el", tree->root->right->word.c_str());
        TEST_ASSERT_EQUAL_STRING("eli", tree->root->right->right->word.c_str());
        TEST_ASSERT_EQUAL_STRING("elia", tree->root->right->right->right->word.c_str());
        TEST_ASSERT_EQUAL_STRING("elian", tree->root->right->right->right->right->word.c_str());
        TEST_ASSERT_EQUAL_STRING("eliane", tree->root->right->right->right->right->right->word.c_str());
    
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds.size());
        TEST_ASSERT_EQUAL_INT(1, tree->root->right->documentIds.size());
        TEST_ASSERT_EQUAL_INT(1, tree->root->right->right->documentIds.size());
        TEST_ASSERT_EQUAL_INT(1, tree->root->right->right->right->documentIds.size());
        TEST_ASSERT_EQUAL_INT(1, tree->root->right->right->right->right->documentIds.size());
        TEST_ASSERT_EQUAL_INT(1, tree->root->right->right->right->right->right->documentIds.size());
    
        TEST_ASSERT_EQUAL_INT(1, tree->root->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(2, tree->root->right->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(3, tree->root->right->right->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(4, tree->root->right->right->right->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(4, tree->root->right->right->right->right->documentIds[0]);
        TEST_ASSERT_EQUAL_INT(3, tree->root->right->right->right->right->right->documentIds[0]);
    
        TEST_ASSERT_EQUAL_INT(0, r1.numComparisons);
        TEST_ASSERT_EQUAL_INT(1, r2.numComparisons);
        TEST_ASSERT_EQUAL_INT(2, r3.numComparisons);
        TEST_ASSERT_EQUAL_INT(3, r4.numComparisons);
        TEST_ASSERT_EQUAL_INT(4, r5.numComparisons);
        TEST_ASSERT_EQUAL_INT(5, r6.numComparisons);
    }
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(TREE::BST::test_one_insertion);
    RUN_TEST(TREE::BST::test_left_tree_insertion);
    RUN_TEST(TREE::BST::test_right_tree_insertion);
    RUN_TEST(TREE::BST::test_generic_tree_insertion);
    RUN_TEST(TREE::BST::test_numbers_and_words_tree_insertion);
    RUN_TEST(TREE::BST::test_different_words_same_doc_tree_insertion);
    RUN_TEST(TREE::BST::test_same_word_same_doc_tree_insertion);
    RUN_TEST(TREE::BST::test_same_word_different_docs_insertion);
    RUN_TEST(TREE::BST::test_similar_words_insertion);
    return UNITY_END();
}
