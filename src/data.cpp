#include <iostream>
#include <vector>
#include <sstream>
#include "data.h"
#include "tree_utils.h"
#include <unordered_set>
#include <string>

namespace DATA {

    static std::string normalise(std::string w) {
    std::string out;
    
    for (char c : w)
    {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c))); //converts to lowercase
        if (std::isalnum(static_cast<unsigned char>(c))) //checks if it's alphanumeric
            out.push_back(c);
    }
    return out;
}

    std::vector<std::string> tokenize(std::string filename){
        std::vector<std::string> fileWords; //vector with the words of the file.
        std::unordered_set<std::string> uniqWords; //set to avoid repeated words in the vector. 
        
        std::ifstream archive(filename); //
        
        if (!archive.is_open()) {
            std::cerr << "Erro ao abrir o arquivo!\n";
            return;
        }
        
        std::string word;
        while(archive>>word){
            if(uniqWords.find(word)==uniqWords.end()){//verifies if the world is already in the vector.
                word = normalise(word);
                fileWords.push_back(word);
                uniqWords.insert(word);
            }
        }

        archive.close();
        return fileWords;
    }

    void buildIndex(const std::string& rootFolder, TREE::BinaryTree* tree);

    std::vector<TREE::Node*> buildNodes(std::string filename){
        std::vector<std::string> fileWords = tokenize(filename);

        std::vector<TREE::Node*> nodeList;

        int index = std::stoi(filename.substr(0, filename.find('.')));

        for(int i=0; i<fileWords.size();i++){
            TREE::Node* node = TREE::createNode(fileWords[i],{index});
            nodeList.push_back(node);
        }

        return nodeList;
    }    

    std::vector<std::string> list_files_txt_in_path(const std::string &dir_path){
    //
    }

    std::string read_file_content(const std::string& full_file_path) { // gabrielle m
        // Open the file and check if it worked
        std::ifstream file_stream(full_file_path);
        if (!file_stream.is_open()) {
            // std::cerr << "Error: Could not open file: " << full_file_path << std::endl;
            return "";
        }
        // Create a stringstream object to act as a buffer
        std::stringstream buffer;
        // Read the content of the file's buffer into the stringstream buffer
        buffer << file_stream.rdbuf();
        // Close the file and get the content from the stringstream buffer as a string
        file_stream.close();
        return buffer.str();
        }
}



