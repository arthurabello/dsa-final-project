#include "tree_utils.h"
#include "../Unity/src/unity.h"

void setUp(void) {}

void tearDown(void) {}

using namespace TREE;

void test_createNode(){
	Node* n = createNode("Nabucodonosor", {1,2,3,4}, 0);

	TEST_ASSERT_NOT_NULL(n);
	TEST_ASSERT_NULL(n->parent);
	TEST_ASSERT_NULL(n->left);
	TEST_ASSERT_NULL(n->right);

	TEST_ASSERT_EQUAL_STRING("Nabucodonosor", n->word.c_str());
	TEST_ASSERT_EQUAL_INT(4, n->documentIds.size());
	TEST_ASSERT_EQUAL_INT(1, n->documentIds[0]);
	TEST_ASSERT_EQUAL_INT(2, n->documentIds[1]);
	TEST_ASSERT_EQUAL_INT(3, n->documentIds[2]);
	TEST_ASSERT_EQUAL_INT(4, n->documentIds[3]);
}

void test_createTree(){
	BinaryTree* tree = createTree();

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_NULL(tree->root);
}

void test_SearchResult(){
    // Search in a empty tree
    BinaryTree* empty = createTree();
    empty->root = nullptr;
    TEST_ASSERT_EQUAL_INT(0, search(empty, "Root").found);

    // Search in a tree with a single node
    BinaryTree* single = createTree();
    single->root = createNode("Root", {1}, 0);
    TEST_ASSERT_EQUAL_INT(1, search(single, "Root").found);

    // Search left child
    BinaryTree* rootLeft = createTree();
    rootLeft->root = createNode("M", {1}, 0);
    rootLeft->root->left = createNode("Left", {1}, 0);
    TEST_ASSERT_EQUAL_INT(1, search(rootLeft, "Left").found);

    // Search right child
    BinaryTree* rootRight = createTree();
    rootRight->root = createNode("M", {1}, 0);
    rootRight->root->right = createNode("Right", {1}, 0);
    TEST_ASSERT_EQUAL_INT(1, search(rootRight, "Right").found);

    // Search one word that is not in the tree
    BinaryTree* notFound = createTree();
    notFound->root = createNode("M", {1}, 0);
    notFound->root->left = createNode("Left", {1}, 0);
    notFound->root->right = createNode("Right", {1}, 0);
    TEST_ASSERT_EQUAL_INT(0, search(notFound, "NotFound").found);
    
    // Search a imcomplete word in the tree
    BinaryTree* incomplete = createTree();
    incomplete->root = createNode("M", {1}, 0);
    incomplete->root->left = createNode("Left", {1}, 0);
    TEST_ASSERT_EQUAL_INT(0, search(incomplete, "Lef").found);

    // Search in a tree with multiple words
    BinaryTree* multiple = createTree();
    multiple->root = createNode("M", {1}, 0);
    multiple->root->left = createNode("Left", {1}, 0);
    multiple->root->right = createNode("Right", {1}, 0);
    multiple->root->left->left = createNode("LeftLeft", {1}, 0);
    multiple->root->left->right = createNode("LeftRight", {1}, 0);
    multiple->root->right->left = createNode("RightLeft", {1}, 0);
    multiple->root->right->right = createNode("RightRight", {1}, 0);
    TEST_ASSERT_EQUAL_INT(1, search(multiple, "RightRight").found);
                         
}

void test_calculateHeight() {
	Node* n = createNode("Parent",{1,2,3}, 0);
	Node* m = createNode("RightSon",{1,2}, 1);
	Node* p = createNode("LeftSon",{1}, 0);
	Node* q = createNode("LeftLeftGrandson",{2,4,5}, 1);

	n->left = p;
	n->right = m;
	p->left = q;

	int height = calculateHeight(n);

	TEST_ASSERT_EQUAL_INT(2, height);
}

void test_updateHeightUp() {
	Node* n = createNode("Parent", {1,2,3}, 0);
	Node* m = createNode("RightSon", {1,2}, 1);
	Node* p = createNode("LeftSon", {1}, 0);
	Node* q = createNode("LeftLeftGrandson", {2,4,5} ,1);

	n->left = p;
	n->right = m;
	p->left = q;

	updateHeightUp(q);

	TEST_ASSERT_EQUAL_INT(q->height, 0);
	TEST_ASSERT_EQUAL_INT(p->height, 0); 
	TEST_ASSERT_EQUAL_INT(n->height, 0);

}

void test_calculateMinDepth() {
    // Empty tree
    Node* empty = nullptr;
    TEST_ASSERT_EQUAL_INT(0, calculateMinDepth(empty));

    // Tree with single node
    Node* single = createNode("Root", {1}, 0);
    TEST_ASSERT_EQUAL_INT(0, calculateMinDepth(single));

    // Balanced tree
    Node* balanced = createNode("Root", {1}, 0);
    balanced->left = createNode("Left", {1}, 0);
    balanced->right = createNode("Right", {1}, 0);
    TEST_ASSERT_EQUAL_INT(1, calculateMinDepth(balanced));

    // Left-skewed tree
    Node* leftSkew = createNode("Root", {1}, 0);
    leftSkew->left = createNode("Left", {1}, 0);
    leftSkew->left->left = createNode("LeftLeft", {1}, 0);
    TEST_ASSERT_EQUAL_INT(2, calculateMinDepth(leftSkew));

    // Right-skewed tree
    Node* rightSkew = createNode("Root", {1}, 0);
    rightSkew->right = createNode("Right", {1}, 0);
    rightSkew->right->right = createNode("RightRight", {1}, 0);
    TEST_ASSERT_EQUAL_INT(2, calculateMinDepth(rightSkew));

    // Tree with one child missing
    Node* incomplete = createNode("Root", {1}, 0);
    incomplete->left = createNode("Left", {1}, 0);
    incomplete->left->right = createNode("LeftRight", {1}, 0);
    TEST_ASSERT_EQUAL_INT(2, calculateMinDepth(incomplete));

	// Tree with multiple child and min depth 3
    Node* depth3 = createNode("Root", {1}, 0);
    depth3->left = createNode("Left", {1}, 0);
    depth3->right = createNode("Right", {1}, 0);
    depth3->left->left = createNode("LeftLeft", {1}, 0);
    depth3->left->right = createNode("LeftRight", {1}, 0);
    depth3->right->left = createNode("RightLeft", {1}, 0);
    depth3->right->right = createNode("RightRight", {1}, 0);
    TEST_ASSERT_EQUAL_INT(2, calculateMinDepth(depth3));
}

void test_countNodes() {
    // Empty tree
    TEST_ASSERT_EQUAL_INT(0, countNodes(nullptr));

    // Single node
    Node* single = createNode("Root", {1}, 0);
    TEST_ASSERT_EQUAL_INT(1, countNodes(single));

    // Multiple nodes
    Node* root = createNode("Root", {1}, 0);
    root->left = createNode("Left", {1}, 0);
    root->right = createNode("Right", {1}, 0);
    TEST_ASSERT_EQUAL_INT(3, countNodes(root));

    // Skewed tree
    Node* skew = createNode("Root", {1}, 0);
    skew->left = createNode("Left", {1}, 0);
    skew->left->left = createNode("LeftLeft", {1}, 0);
    skew->left->left->left = createNode("LeftLeftLeft", {1}, 0);
    TEST_ASSERT_EQUAL_INT(4, countNodes(skew));
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_createNode);
    RUN_TEST(test_createTree);
    RUN_TEST(test_SearchResult);
    RUN_TEST(test_calculateHeight);
    RUN_TEST(test_updateHeightUp);
	  RUN_TEST(test_calculateMinDepth);
    RUN_TEST(test_countNodes);
    return UNITY_END();
}
