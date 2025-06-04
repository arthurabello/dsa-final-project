#include <iostream>
#include <vector>
#include <sstream>
#include "data.h"
#include "tree_utils.h"
#include <unordered_set>
#include <string>
#include <filesystem>
#include <cctype>

namespace DATA {

    static std::string normalise(const std::string& w) { 
        std::string out;

        for (unsigned char c : w) {
            if (c > 127) { //checks if non-ascii stuff is given
                return ""; //empty string indicates an error
            }
            char lower_c = static_cast<char>(std::tolower(c)); //converts to lowercase
            if (std::isalnum(static_cast<unsigned char>(lower_c))) { //only alphanumeric characters
                out.push_back(lower_c);
            }
        }
        return out;
    }

    std::vector<std::string> tokenize(std::string filename){
        std::vector<std::string> fileWords; //vector with the words of the file.
        std::unordered_set<std::string> uniqWords; //set to avoid repeated words in the vector. 
        
        std::ifstream archive(filename); //
        
        if (!archive.is_open()) {
            std::cerr << "Erro ao abrir o arquivo!\n";
            return std::vector<std::string>();
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

    std::vector<std::string> list_files_txt_in_path(const std::string &dir_path) {

        std::vector<std::string> txt_files;
        namespace fs = std::filesystem; //shortcut
        fs::path path(dir_path); //checks if directory exists and is a directory
        if (!fs::exists(path) || !fs::is_directory(path)) {
            return txt_files;  //empty

        } else {
            for (const auto& directory_entry : fs::directory_iterator(path)) { //iterates through the directory
                if (directory_entry.is_regular_file()) {
                    auto filename = directory_entry.path().filename().string();
                    auto ext = directory_entry.path().extension().string();
                    if (ext.size() == 4 && //case-insensitive check for ".txt"
                        (ext == ".txt" || ext == ".TXT" || ext == ".Txt" || ext == ".tXt" || ext == ".txT" /* etc */)) {
                        txt_files.push_back(filename);
                    }
                }
        }
        return txt_files;
        }
    }

    std::string read_file_content(const std::string& full_file_path) {
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



