#include <iostream>
#include <bst.h>
#include <cassert>

namespace TREE::BST{

void test_one_insertion() {
    BinaryTree* tree = createTree();
    InsertResult result = insert(tree, "arvore", 1);

    assert(tree->root != nullptr);
    assert(tree->root->word == "arvore");
    assert(tree->root->documentIds.size() == 1);
    assert(tree->root->documentIds[0] == 1);
    assert(result.numComparisons == 0);
}

void test_left_tree_insertion() {
    BinaryTree* tree = createTree();

    InsertResult r1 = insert(tree, "zebra", 1);
    InsertResult r2 = insert(tree, "yak", 2);
    InsertResult r3 = insert(tree, "xexeu", 3);

    assert(tree->root->word == "zebra");
    assert(tree->root->left->word == "yak");
    assert(tree->root->left->left->word == "xexeu");

    assert(tree->root->documentIds[0] == 1);
    assert(tree->root->left->documentIds[0] == 2);
    assert(tree->root->left->left->documentIds[0] == 3);

    assert(r1.numComparisons == 0);
    assert(r2.numComparisons == 1);
    assert(r3.numComparisons == 2);
}

void test_right_tree_insertion() {
    BinaryTree* tree = createTree();

    InsertResult r1 = insert(tree, "arara", 1);
    InsertResult r2 = insert(tree, "baleia", 2);
    InsertResult r3 = insert(tree, "cachorro", 3);

    assert(tree->root->word == "arara");
    assert(tree->root->right->word == "baleia");
    assert(tree->root->right->right->word == "cachorro");

    assert(tree->root->documentIds[0] == 1);
    assert(tree->root->right->documentIds[0] == 2);
    assert(tree->root->right->right->documentIds[0] == 3);

    assert(r1.numComparisons == 0);
    assert(r2.numComparisons == 1);
    assert(r3.numComparisons == 2);
}

void test_generic_tree_insertion() {
    BinaryTree* tree = createTree();

    InsertResult r1 = insert(tree, "macaco", 1);
    InsertResult r2 = insert(tree, "girafa", 2);
    InsertResult r3 = insert(tree, "tigre", 3);
    InsertResult r4 = insert(tree, "jabuti", 4);
    InsertResult r5 = insert(tree, "panda", 5);

    assert(tree->root->word == "macaco");
    assert(tree->root->left->word == "girafa");
    assert(tree->root->left->right->word == "jabuti");
    assert(tree->root->right->word == "tigre");
    assert(tree->root->right->left->word == "panda");

    assert(tree->root->documentIds[0] == 1);
    assert(tree->root->left->documentIds[0] == 2);
    assert(tree->root->left->right->documentIds[0] == 4);
    assert(tree->root->right->documentIds[0] == 3);
    assert(tree->root->right->left->documentIds[0] == 5);

    assert(r1.numComparisons == 0);
    assert(r2.numComparisons == 1);
    assert(r3.numComparisons == 1);
    assert(r4.numComparisons == 2);
    assert(r5.numComparisons == 2);
}

void test_numbers_and_words_tree_insertion() {
    BinaryTree* tree = createTree();

    InsertResult r1 = insert(tree, "12345", 1);
    InsertResult r2 = insert(tree, "sucuri", 2);
    InsertResult r3 = insert(tree, "23450", 3);
    InsertResult r4 = insert(tree, "ornitorrinco", 4);

    assert(tree->root->word == "12345");
    assert(tree->root->right->word == "sucuri");
    assert(tree->root->right->left->word == "23450");
    assert(tree->root->right->left->left->word == "ornitorrinco");

    assert(tree->root->documentIds[0] == 1);
    assert(tree->root->right->documentIds[0] == 2);
    assert(tree->root->right->left->documentIds[0] == 3);
    assert(tree->root->right->left->left->documentIds[0] == 4);

    assert(r1.numComparisons == 0);
    assert(r2.numComparisons == 1);
    assert(r3.numComparisons == 2);
    assert(r4.numComparisons == 3);
}

void test_different_words_same_doc_tree_insertion(){
    BinaryTree* tree = createTree();
    InsertResult r1 = insert(tree, "cobra", 1);
    InsertResult r2 = insert(tree, "jararaca", 1);

    assert(tree->root->word == "cobra");
    assert(tree->root->right->word == "jararaca");

    assert(tree->root->documentIds[0] == 1);
    assert(tree->root->right->documentIds[0] == 1);

    assert(tree->root->documentIds.size() == 1);
    assert(tree->root->right->documentIds.size() == 1);

    assert(r1.numComparisons == 0);
    assert(r2.numComparisons == 1);
}

void test_same_word_same_doc_tree_insertion(){
    BinaryTree* tree = createTree();
    InsertResult r1 = insert(tree, "capivara", 1);
    InsertResult r2 = insert(tree, "capivara", 1);

    assert(tree->root->word == "capivara");
    assert(tree->root->documentIds[0] == 1);
    assert(tree->root->documentIds.size() == 1);
    assert(tree->root->left == nullptr);
    assert(tree->root->right == nullptr);

    assert(r1.numComparisons == 0);
    assert(r2.numComparisons == 1);
}

void test_same_word_different_docs_insertion(){
    BinaryTree* tree = createTree();
    InsertResult r1 = insert(tree, "tucano", 1);
    InsertResult r2 = insert(tree, "tucano", 2);

    assert(tree->root->word == "tucano");
    assert(tree->root->documentIds[0] == 1);
    assert(tree->root->documentIds[1] == 2);
    assert(tree->root->documentIds.size() == 1);
    assert(tree->root->left == nullptr);
    assert(tree->root->right == nullptr);

    assert(r1.numComparisons == 0);
    assert(r2.numComparisons == 1);
}

void test_similar_words_insertion() {
    BinaryTree* tree = createTree();

    InsertResult r1 = insert(tree, "e", 1);
    InsertResult r2 = insert(tree, "el", 2);
    InsertResult r3 = insert(tree, "eli", 3);
    InsertResult r4 = insert(tree, "elia", 4);
    InsertResult r5 = insert(tree, "elian", 4);
    InsertResult r6 = insert(tree, "eliane", 3);

    assert(tree->root->word == "e");
    assert(tree->root->right->word == "el");
    assert(tree->root->right->right->word == "eli");
    assert(tree->root->right->right->right->word == "elia");
    assert(tree->root->right->right->right->right->word == "elian");
    assert(tree->root->right->right->right->right->right->word == "eliane");

    assert(tree->root->documentIds.size() == 1);
    assert(tree->root->right->documentIds.size() == 1);
    assert(tree->root->right->right->documentIds.size() == 1);
    assert(tree->root->right->right->right->documentIds.size() == 1);
    assert(tree->root->right->right->right->right->documentIds.size() == 1);
    assert(tree->root->right->right->right->right->right->documentIds.size() == 1);

    assert(tree->root->documentIds[0] == 1);
    assert(tree->root->right->documentIds[0] == 2);
    assert(tree->root->right->right->documentIds[0] == 3);
    assert(tree->root->right->right->right->documentIds[0] == 4);
    assert(tree->root->right->right->right->right->documentIds[0] == 4);
    assert(tree->root->right->right->right->right->right->documentIds[0] == 3);

    assert(r1.numComparisons == 0);
    assert(r2.numComparisons == 1);
    assert(r3.numComparisons == 2);
    assert(r4.numComparisons == 3);
    assert(r5.numComparisons == 4);
    assert(r6.numComparisons == 5);
}

}
