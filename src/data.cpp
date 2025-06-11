#include <iostream>
#include <vector>
#include "data.h"
#include <string>
#include <fstream>
#include <cctype>

namespace DATA {
    // Makes a string lowercase and removes non-alphanumeric characters
    std::string normalise(const std::string& w) { 
        std::string out;

        for (unsigned char c : w) {
            // Converts to lowercase
            char lower_c = std::tolower(c);

            if (std::isalnum(lower_c)) {
                out.push_back(lower_c);
            }
        }

        return out;
    }

    // Breaks a text file into a vector of normalized "words"
    std::vector<std::string> tokenize(std::string filename) {
        std::vector<std::string> words;
        
        std::ifstream archive(filename);
        
        if (!archive.is_open()) {
            std::cerr << "Error: opening file: " << filename << std::endl;
            return std::vector<std::string>();
        }
        
        std::string word;
        while(archive >> word) {
            // Verifies if the word isn't already in the vector
            bool found = false;
            for (auto it = words.begin(); it != words.end(); ++it) {
                if (*it == word) {
                    found = true;
                    break;
                }
            }

            if (!found) words.push_back(normalise(word));
        }

        archive.close();
        return words;
    }

    // Finds all files inside a directory and returns its names
    std::vector<std::string> list_txt_files_in_path(const std::string& filename) {
        std::vector<std::string> files;
        int index = 1;

        while(true){
            std::string file_name = filename + std::to_string(index) + ".txt";
            std::ifstream file(file_name);

            if (!file.is_open()) {
                std::cerr << "Error: unable to open file " << file_name << " for reading." << std::endl;
                break;
            }

            files.push_back(file_name);
            file.close();
            index++;
        }

        return files;
    }
}
