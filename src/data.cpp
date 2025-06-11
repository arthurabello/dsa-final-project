#include <iostream>
#include <vector>
#include "data.h"
#include <string>
#include <filesystem>
#include <cctype>
#include <algorithm>

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

    std::vector<std::string> tokenize(std::string filename){
        std::vector<std::string> words;
        
        std::ifstream archive(filename);
        
        if (!archive.is_open()) {
            std::cerr << "Erro ao abrir o arquivo!" << std::endl;
            return std::vector<std::string>();
        }
        
        std::string word;
        while(archive >> word) {
            // Verifies if the word isn't already in the vector
            if(std::find(words.begin(), words.end(), word) == words.end()) {
                words.push_back(normalise(word));
            }
        }

        archive.close();
        return words;
    }

    std::vector<std::string> list_txt_files_in_path(const std::string &dir_path) {
        std::vector<std::string> txt_files;

        namespace fs = std::filesystem; // Shortcut

        // Checks if path exists and is a directory
        fs::path path(dir_path);
        if (fs::exists(path) && fs::is_directory(path)) {
            // Iterates through the directory
            for (const auto& directory_entry : fs::directory_iterator(path)) {
                if (directory_entry.is_regular_file()) {
                    auto filename = directory_entry.path().filename().string();
                    auto ext = directory_entry.path().extension().string();

                    if (ext == ".txt") {
                        txt_files.push_back(filename);
                    }
                }
            }

        }

        return txt_files;
    }
}
