#include "avl.h"
#include "../../Unity/src/unity.h"

void setUp(void) {}

void tearDown(void) {}

using namespace TREE;

void test_LL_rotation {
	BinaryTree* tree = createTree();

	InsertResult first = AVL::insert(*tree,"A",1);
	InsertResult second = AVL::insert(*tree,"B",2);
	InsertResult third = AVL::insert(*tree,"C",3);

	TEST_ASSERT_EQUAL_STRING(tree->root->word.c_str(),"B");
	TEST_ASSERT_EQUAL_STRING(tree->root->left->word.c_str(),"A");
	TEST_ASSERT_EQUAL_STRING(tree->root->right->word.c_str(),"C");
	TEST_ASSERT_EQUAL_INT(tree->root->height, 1);
	
}

void test_RR_rotation() {
	BinaryTree* tree = createTree();
	
	InsertResult first = AVL::insert(*tree,"C",1);
	InsertResult second = AVL::insert(*tree,"B",2);
	InsertResult third = AVL::insert(*tree,"A",3);	
	
	TEST_ASSERT_EQUAL_STRING(tree->root->word.c_str(),"B");
	TEST_ASSERT_EQUAL_STRING(tree->root->left->word.c_str(),"A");
	TEST_ASSERT_EQUAL_STRING(tree->root->right->word.c_str(),"C");
	TEST_ASSERT_EQUAL_INT(tree->root->height, 1);	
	
}

void test_LR_rotation() {
	BinaryTree* tree = createTree();
	
	InsertResult first = AVL::insert(*tree,"C",1);
	InsertResult second = AVL::insert(*tree,"A",2);
	InsertResult third = AVL::insert(*tree,"B",3);
	
	TEST_ASSERT_EQUAL_STRING(tree->root->word.c_str(),"B");
	TEST_ASSERT_EQUAL_STRING(tree->root->left->word.c_str(),"A");
	TEST_ASSERT_EQUAL_STRING(tree->root->right->word.c_str(),"C");
	TEST_ASSERT_EQUAL_INT(tree->root->height, 1);
	
}

void test_RL_rotation() {
	BinaryTree* tree = createTree();
	
	InsertResult first = AVL::insert(*tree,"A",1);
	InsertResult second = AVL::insert(*tree,"C",2);
	InsertResult third = AVL::insert(*tree,"B",3);
	
	TEST_ASSERT_EQUAL_STRING(tree->root->word.c_str(),"B");
	TEST_ASSERT_EQUAL_STRING(tree->root->left->word.c_str(),"A");
	TEST_ASSERT_EQUAL_STRING(tree->root->right->word.c_str(),"C");
	TEST_ASSERT_EQUAL_INT(tree->root->height, 1);
}

void test_repeated_words(){
	BinaryTree* tree = createTree();
	
	InsertResult first = AVL::insert(*tree,"A",1);
	InsertResult second = AVL::insert(*tree,"A",2);
	InsertResult third = AVL::insert(*tree,"C",3);
	InsertResult fourth = AVL::insert(*tree,"A",4);
	InsertResult fifth = AVL::insert(*tree,"B",2);
	InsertResult sixth = AVL::insert(*tree,"C",5);
	
	
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
    RUN_TEST(test_getAverageInsertionTime);
    RUN_TEST(test_getAverageComparisonsPerInsertion);
    RUN_TEST(test_getAverageSearchTime);
    RUN_TEST(test_getAverageComparisonsPerSearch);
    RUN_TEST(test_balanceMetrics);
    return UNITY_END();

}
