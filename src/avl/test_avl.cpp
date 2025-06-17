#include "avl.h"
#include "../../Unity/src/unity.h"

void setUp(void) {}

void tearDown(void) {}

namespace TREE::AVL {
	
	void test_LL_rotation() {
		BinaryTree* tree = createTree();

		InsertResult first = insert(*tree,"A",1);
		InsertResult second = insert(*tree,"B",2);
		InsertResult third = insert(*tree,"C",3);

		TEST_ASSERT_EQUAL_STRING(tree->root->word.c_str(),"B");
		TEST_ASSERT_EQUAL_STRING(tree->root->left->word.c_str(),"A");
		TEST_ASSERT_EQUAL_STRING(tree->root->right->word.c_str(),"C");
		TEST_ASSERT_EQUAL_INT(tree->root->height, 1);
	}

	void test_RR_rotation() {
		BinaryTree* tree = createTree();
		
		InsertResult first = insert(*tree,"C",1);
		InsertResult second = insert(*tree,"B",2);
		InsertResult third = insert(*tree,"A",3);	
		
		TEST_ASSERT_EQUAL_STRING(tree->root->word.c_str(),"B");
		TEST_ASSERT_EQUAL_STRING(tree->root->left->word.c_str(),"A");
		TEST_ASSERT_EQUAL_STRING(tree->root->right->word.c_str(),"C");
		TEST_ASSERT_EQUAL_INT(tree->root->height, 1);	
	}

	void test_LR_rotation() {
		BinaryTree* tree = createTree();
		
		InsertResult first = insert(*tree,"C",1);
		InsertResult second = insert(*tree,"A",2);
		InsertResult third = insert(*tree,"B",3);
		
		TEST_ASSERT_EQUAL_STRING(tree->root->word.c_str(),"B");
		TEST_ASSERT_EQUAL_STRING(tree->root->left->word.c_str(),"A");
		TEST_ASSERT_EQUAL_STRING(tree->root->right->word.c_str(),"C");
		TEST_ASSERT_EQUAL_INT(tree->root->height, 1);
	}

	void test_RL_rotation() {
		BinaryTree* tree = createTree();
		
		InsertResult first = insert(*tree,"A",1);
		InsertResult second = insert(*tree,"C",2);
		InsertResult third = insert(*tree,"B",3);
		
		TEST_ASSERT_EQUAL_STRING(tree->root->word.c_str(),"B");
		TEST_ASSERT_EQUAL_STRING(tree->root->left->word.c_str(),"A");
		TEST_ASSERT_EQUAL_STRING(tree->root->right->word.c_str(),"C");
		TEST_ASSERT_EQUAL_INT(tree->root->height, 1);
	}

	void test_repeated_words() {
		BinaryTree* tree = createTree();
		
		InsertResult first = insert(*tree,"A",1);
		InsertResult second = insert(*tree,"A",2);
		InsertResult third = insert(*tree,"C",3);
		InsertResult fourth = insert(*tree,"A",4);
		InsertResult fifth = insert(*tree,"B",2);
		InsertResult sixth = insert(*tree,"C",5);
		
		TEST_ASSERT_EQUAL_STRING(tree->root->word.c_str(),"B");
		TEST_ASSERT_EQUAL_INT(tree->root->documentIds[0],2);
		TEST_ASSERT_EQUAL_INT(tree->root->right->documentIds[0],3);
		TEST_ASSERT_EQUAL_INT(tree->root->right->documentIds[1],5);
		TEST_ASSERT_EQUAL_INT(tree->root->left->documentIds[0],1);
		TEST_ASSERT_EQUAL_INT(tree->root->left->documentIds[1],2);
		TEST_ASSERT_EQUAL_INT(tree->root->left->documentIds[2],4);
	}
}


int main() {

    UNITY_BEGIN();
    RUN_TEST(TREE::AVL::test_LL_rotation);
    RUN_TEST(TREE::AVL::test_LR_rotation);
    RUN_TEST(TREE::AVL::test_RL_rotation);
    RUN_TEST(TREE::AVL::test_RR_rotation);
    RUN_TEST(TREE::AVL::test_repeated_words);
    return UNITY_END();

}
