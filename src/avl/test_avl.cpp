#include "avl.h"
#include <vector>

using namespace TREE;

void printPreOrder(Node* root) {
	if (root != nullptr) {
	std::cout << root->word << "\n";
	printPreOrder(root->left);
	printPreOrder(root->right);
	return;
	}
}

void testOne() {
	BinaryTree* tree = createTree();

	InsertResult first = AVL::insert(*tree,"A",1);
	InsertResult second = AVL::insert(*tree,"B",2);
	InsertResult third = AVL::insert(*tree,"C",3);
	printPreOrder(tree->root);


	std::cout<<"Essa é a raiz:"<< tree->root->word<<"\n";
	std::cout<< "Esse é a altura da raiz:" << tree->root->height<<"\n";
	std::cout<< "Esse o balanceamento da raiz:" << AVL::bf(tree->root)<<"\n";

	InsertResult fourth = AVL::insert(*tree,"J",3);
	InsertResult fifth= AVL::insert(*tree,"F",3);
	InsertResult sixth = AVL::insert(*tree,"E",3);
	InsertResult seventh = AVL::insert(*tree,"I",3);
	InsertResult eighth = AVL::insert(*tree,"H",3);
	InsertResult nineth= AVL::insert(*tree,"G",3);

	std::cout<<"Essa é a raiz:"<< tree->root->word<<"\n";
	std::cout<< "Esse é a altura da raiz:" << tree->root->height<<"\n";
	std::cout<< "Esse o balanceamento da raiz:" << AVL::bf(tree->root)<<"\n";

	printPreOrder(tree->root);
	
}

void testTwo(){
	BinaryTree* tree = createTree();
	
	InsertResult first = AVL::insert(*tree,"A",1);
	InsertResult second = AVL::insert(*tree,"A",2);
	InsertResult third = AVL::insert(*tree,"C",3);
	InsertResult fourth = AVL::insert(*tree,"A",4);
	InsertResult fifth = AVL::insert(*tree,"B",2);
	InsertResult sixth = AVL::insert(*tree,"G",3);
	
	printPreOrder(tree->root);
	
	// InsertResult seventh = AVL::insert(*tree,"K",15);
	// InsertResult eighth = AVL::insert(*tree,"L",25);
	// InsertResult nineth = AVL::insert(*tree,"Y",34);
	
	// printPreOrder(tree->root);
	
	// InsertResult tenth = AVL::insert(*tree,"X",15);
	// InsertResult eleventh = AVL::insert(*tree,"D",23);
	// InsertResult twelveth = AVL::insert(*tree,"F",312);

	// printPreOrder(tree->root);

	// InsertResult thirtheenth= AVL::insert(*tree,"H",100);
	// InsertResult fourteenth = AVL::insert(*tree,"I",20);
	// InsertResult fifteenth = AVL::insert(*tree,"K",4);

	// printPreOrder(tree->root);

	// InsertResult sixteenth = AVL::insert(*tree,"G",1);
	// InsertResult seventeenth = AVL::insert(*tree,"T",7);
	// InsertResult eighteenth = AVL::insert(*tree,"X",9);

	// printPreOrder(tree->root);

	// InsertResult nineteenth = AVL::insert(*tree,"B",12);
	// InsertResult twentieth = AVL::insert(*tree,"B",21);
	// InsertResult twentyFirst = AVL::insert(*tree,"C",33);	
	
	// printPreOrder(tree->root);
	
}




int main() {

	
// testOne();
testTwo();

}
