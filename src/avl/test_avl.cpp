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
	BinaryTree* tree1 = createTree();

	InsertResult first = AVL::insert(*tree1,"A",1);
	InsertResult second = AVL::insert(*tree1,"B",2);
	InsertResult third = AVL::insert(*tree1,"C",3);
	printPreOrder(tree1->root);


	std::cout<<"Essa é a raiz:"<< tree1->root->word<<"\n";
	std::cout<< "Esse é a altura da raiz:" << tree1->root->height<<"\n";
	std::cout<< "Esse o balanceamento da raiz:" << AVL::bf(tree1->root)<<"\n";

	InsertResult fourth = AVL::insert(*tree1,"J",3);
	InsertResult fifth= AVL::insert(*tree1,"F",3);
	InsertResult sixth = AVL::insert(*tree1,"E",3);
	InsertResult seventh = AVL::insert(*tree1,"I",3);
	InsertResult eighth = AVL::insert(*tree1,"H",3);
	InsertResult nineth= AVL::insert(*tree1,"G",3);

	std::cout<<"Essa é a raiz:"<< tree1->root->word<<"\n";
	std::cout<< "Esse é a altura da raiz:" << tree1->root->height<<"\n";
	std::cout<< "Esse o balanceamento da raiz:" << AVL::bf(tree1->root)<<"\n";

	printPreOrder(tree1->root);
	
}



int main() {

	
testOne();

}
