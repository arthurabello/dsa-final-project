#include <iostream>
#include <vector>
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
}
