// main_bst.cpp
#include <iostream> // for std::cout, std::cerr
#include <string>   // for std::string, std::stoi
#include <vector>
#include "../data.h"
#include "bst.h"

using namespace TREE; // is this right?l

int main(int argc, char *argv[]){  //gabrielle m

    // Verifying if there is correctly 4 arguments
    //Didnt get it, because the number of archives must be the number
    //of directories that were put in the command line.
    if(argc!=4){ 
        std::cerr << "Using: ./bst <search|stats> <n_docs> <diretorio>" << std::endl;
        return 1; //error code
    } 

    // Getting args
    std::string command = argv[1];
    std::string n_docs_str = argv[2];
    std::string dir_path = argv[3];

    // Converting n_docs_str to int
    int n_docs_int;
    try{
        n_docs_int = std::stoi(n_docs_str); // string to int
        if(n_docs_int<=0){
            std::cerr << "Error <n_docs> invalid. It must be positive" << std::endl;
            return 1;
        }
    } catch (const std::invalid_argument& ia){
        std::cerr << "Error <n_docs> invalid. It must be a positive integer" << std::endl;
        return 1;
    } catch (const std::out_of_range& oot){
        std::cerr << "Error <n_docs> invalid. It is out of range" << std::endl;
        return 1;
    }

    // Printing infos
    std::cout << "Command: " << command << ", Number of documents: " << n_docs_int << ", Directory: " << std::endl;
    

    // TODO: main logic
    // Reading files according to n_docs and dir

    // Build inverted indices
    //gabriel the sheep

    BinaryTree *tree = createTree();
    std::vector<InsertResult> listOfResults;
    for(int i = 0; i<n_docs_int;i++){
        std::string path = "../data/"+ dir_path[3+i] + ".txt" ;

        std::vector<std::string> listOfWords = DATA::tokenize(path);

        for(int i = 0; i<listOfWords.size();i++){
            InsertResult result = BST::insert(tree,listOfWords[i],int(dit_path))


        }

    }

    BinaryTree *tree = createTree();

    for()

    
    // Running search in stats or search only
    if(command == "stats"){
        // TODO: show statistics 
    } else if (command == "search"){
        // TODO: get it in loop of word search
        // read choosen word
        // search in BST
        // show results
    } else{
        std::cerr << "Error: Unkowned command: " << command << ". Use 'search' or 'stats'." << std::endl;
        return 1;
    }
    
    return 0;
}