#include <iostream>
#include <vector>
#include "data.h"
#include <string>
#include <filesystem>
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
            std::cerr << "Erro ao abrir o arquivo!" << std::endl;
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
    std::vector<std::string> list_txt_files_in_path(const std::string &dir_path) {
        std::vector<std::string> files;

        namespace fs = std::filesystem; // Shortcut

        // Checks if path exists and is a directory
        fs::path path(dir_path);
        if (fs::exists(path) && fs::is_directory(path)) {
            // Iterates through the directory
            for (const auto& dir_entry : fs::directory_iterator(path)) {
                if (dir_entry.is_regular_file()) {
                    auto filename = dir_entry.path().filename().string();
                    auto ext = dir_entry.path().extension().string();

                    if (ext == ".txt") {
                        files.push_back(filename);
                    }
                }
            }

        }

        return files;
    }
}
