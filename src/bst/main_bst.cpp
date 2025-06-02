// main_bst.cpp
#include <iostream> 
#include <string>   
#include <chrono>

#include "data.cpp"
#include "bst.cpp"
#include "bst.h"

using namespace TREE::BST; // is this right?

int main(int argc, char *argv[]){  // gabrielle m

    // Verifying if there is correctly 4 arguments
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


    // Initialize bst tree
    TREE::BinaryTree* bst_tree = TREE::createTree();

    // Get List of .txt files
    std::vector<std::string> all_filenames = DATA::list_files_txt_in_path(dir_path);
    if (all_filenames.empty()) { 
        /* ... handle empty or error ... */ 
    }

    // Seeing if there is enough files to process n_docs_int
    int files_to_process_count;
    if(all_filenames.size() <= n_docs_int){
        files_to_process_count = all_filenames.size();
        std::cout << "Processing the firsts " << files_to_process_count << " docs. There were only " << files_to_process_count << "files available." << std::endl;
    }else{
        files_to_process_count = n_docs_int;
        std::cout << "Processing the firsts " << files_to_process_count << " docs." << std::endl;
    }

    // Loop Through Selected Files for Indexing 
    for (int i = 0; i < files_to_process_count; ++i) {
        
        // Making logic to acess file (path + its name = whole path)
        std::string& filename_only = all_filenames[i];
        std::string full_path_to_file = dir_path;
        if(!dir_path.empty() && dir_path.back() != '/' && dir_path.back() != '\\'){ // add a "universal" bar at the end in case it's not there
            full_path_to_file += "/";
        }
        full_path_to_file += filename_only;

        // Getting a vector of tokens out of content read
        std::vector<std::string> words_in_doc = DATA::tokenize(full_path_to_file);
        // maybe here do another check if tokenize works really

        // Iter in each token of current file being analized
        for(const std::string& word : words_in_doc){
            // call for bst insert

            // stats (later)
        }

    }



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
    

    // HERE ALLOC MEMORY AND DELETE TREE!!!

    return 0;
}