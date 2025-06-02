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
	//TBI
}

void test_calculateHeight{
	Node* n = createNode("Parent",{1,2,3},0);
	Node* m = createNode("RightSon",{1,2},1);
	Node* p = createNode("LeftSon",{1},0);
	Node* q = createNode("LeftLeftGrandson",{2,4,5},1)
	
	n->left = p;
	n->right = m;
	p->left = q;
	
	int height = calculateHeight(n);
	
	TEST_ASSERT_EQUAL_INT(3, height);
}

void test_updateHeightUp{
	Node* n = createNode("Parent",{1,2,3},0);
	Node* m = createNode("RightSon",{1,2},1);
	Node* p = createNode("LeftSon",{1},0);
	Node* q = createNode("LeftLeftGrandson",{2,4,5},1)
	
	n->left = p;
	n->right = m;
	p->left = q;
	
	updateHeightUp(q);
	
	TEST_ASSERT_EQUAL_INT(q->height, 0);
	TEST_ASSERT_EQUAL_INT(p->height, 1);
	TEST_ASSERT_EQUAL_INT(n->height, 2);
	
}