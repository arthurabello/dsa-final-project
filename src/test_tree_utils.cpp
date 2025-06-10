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

void test_calculateHeight() {
	Node* n = createNode("Parent",{1,2,3}, 0);
	Node* m = createNode("RightSon",{1,2}, 1);
	Node* p = createNode("LeftSon",{1}, 0);
	Node* q = createNode("LeftLeftGrandson",{2,4,5}, 1);
	
	n->left = p;
	n->right = m;
	p->left = q;
	
	int height = calculateHeight(n);
	
	TEST_ASSERT_EQUAL_INT(3, height);
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
	TEST_ASSERT_EQUAL_INT(p->height, 1);
	TEST_ASSERT_EQUAL_INT(n->height, 1);
	
}

void test_calculateMinDepth() {
    // Empty tree
    Node* empty = nullptr;
    TEST_ASSERT_EQUAL_INT(0, calculateMinDepth(empty));

    // Tree with single node
    Node* single = createNode("Root", {1}, 0);
    TEST_ASSERT_EQUAL_INT(1, calculateMinDepth(single));

    // Balanced tree 
    Node* balanced = createNode("Root", {1}, 0);
    balanced->left = createNode("Left", {1}, 0);
    balanced->right = createNode("Right", {1}, 0);
    TEST_ASSERT_EQUAL_INT(2, calculateMinDepth(balanced));

    // Left-skewed tree
    Node* leftSkew = createNode("Root", {1}, 0);
    leftSkew->left = createNode("Left", {1}, 0);
    leftSkew->left->left = createNode("LeftLeft", {1}, 0);
    TEST_ASSERT_EQUAL_INT(3, calculateMinDepth(leftSkew));

    // Right-skewed tree
    Node* rightSkew = createNode("Root", {1}, 0);
    rightSkew->right = createNode("Right", {1}, 0);
    rightSkew->right->right = createNode("RightRight", {1}, 0);
    TEST_ASSERT_EQUAL_INT(3, calculateMinDepth(rightSkew));

    // Tree with one child missing
    Node* incomplete = createNode("Root", {1}, 0);
    incomplete->left = createNode("Left", {1}, 0);
    incomplete->left->right = createNode("RightRight", {1}, 0);
    TEST_ASSERT_EQUAL_INT(2, calculateMinDepth(incomplete));

	// Tree with multiple child and min depth 3
    Node* depth3 = createNode("Root", {1}, 0);
    depth3->left = createNode("Left", {1}, 0);
    depth3->right = createNode("Right", {1}, 0);
    depth3->left->left = createNode("LeftLeft", {1}, 0);
    depth3->left->right = createNode("LeftRight", {1}, 0);
    depth3->right->left = createNode("RightLeft", {1}, 0);
    depth3->right->right = createNode("RightRight", {1}, 0);
    TEST_ASSERT_EQUAL_INT(3, calculateMinDepth(depth3));
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

void test_getAverageInsertionTime() {
    AggregateStats stats;
    stats.total_words_processed = 0;
    stats.sum_of_insertion_times_ms = 100.0;
    TEST_ASSERT_EQUAL_DOUBLE(0.0, getAverageInsertionTime(stats));

    stats.total_words_processed = 5;
    stats.sum_of_insertion_times_ms = 50.0;
    TEST_ASSERT_EQUAL_DOUBLE(10.0, getAverageInsertionTime(stats));

    stats.total_words_processed = 10;
    stats.sum_of_insertion_times_ms = 200.0;
    TEST_ASSERT_EQUAL_DOUBLE(20.0, getAverageInsertionTime(stats));
}

void test_getAverageComparisonsPerInsertion() {
    AggregateStats stats;
    stats.total_words_processed = 0;
    stats.total_comparisons_insertion = 50;
    TEST_ASSERT_EQUAL_DOUBLE(0.0, getAverageComparisonsPerInsertion(stats));

    stats.total_words_processed = 5;
    stats.total_comparisons_insertion = 50;
    TEST_ASSERT_EQUAL_DOUBLE(10.0, getAverageComparisonsPerInsertion(stats));

    stats.total_words_processed = 20;
    stats.total_comparisons_insertion = 100;
    TEST_ASSERT_EQUAL_DOUBLE(5.0, getAverageComparisonsPerInsertion(stats));
}

void test_getAverageSearchTime() {
    AggregateStats stats;
    stats.total_searches = 0;
    stats.sum_of_search_times_ms = 120.0;
    TEST_ASSERT_EQUAL_DOUBLE(0.0, getAverageSearchTime(stats));

    stats.total_searches = 4;
    stats.sum_of_search_times_ms = 100.0;
    TEST_ASSERT_EQUAL_DOUBLE(25.0, getAverageSearchTime(stats));

    stats.total_searches = 8;
    stats.sum_of_search_times_ms = 400.0;
    TEST_ASSERT_EQUAL_DOUBLE(50.0, getAverageSearchTime(stats));
}

void test_getAverageComparisonsPerSearch() {
    AggregateStats stats;
    stats.total_searches = 0;
    stats.total_comparisons_search = 50;
    TEST_ASSERT_EQUAL_DOUBLE(0.0, getAverageComparisonsPerSearch(stats)); 

    stats.total_searches = 5;
    stats.total_comparisons_search = 100;
    TEST_ASSERT_EQUAL_DOUBLE(20.0, getAverageComparisonsPerSearch(stats));

    stats.total_searches = 10;
    stats.total_comparisons_search = 300;
    TEST_ASSERT_EQUAL_DOUBLE(30.0, getAverageComparisonsPerSearch(stats));
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
    return UNITY_END();
}