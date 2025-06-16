#ifndef DATA_H
#define DATA_H
#include <iostream>
#include <vector>
#include <fstream>
#include "tree_utils.h"

namespace DATA {
    /**
     * @brief Makes a string lowercase and removes non-alphanumeric characters.
     * 
     * @param string Text you wish to normalise
     * @returns Normalised string
     */
    std::string normalise(const std::string& w);
    
    /**
     * @brief Breaks a text file into a vector of normalized "words".
     *
     * This function expects the file to have no punctuation and will only break
     * the words at spaces.
     *
     * @param filename Path to the file with the words you want
     * @returns A vector with all the words from the file
     */
    std::vector<std::string> tokenize(std::string filename);

    /**
     * @brief Retrieves a list of all .txt filenames from a specified directory.
     *
     * This function scans the given directory path and returns a vector
     * containing the names (not full paths) of all files ending with the ".txt"
     * extension. Returns an empty vector if the directory cannot be accessed or
     * no .txt files are found.
     *
     * @param path The path to the directory to scan
     * @returns A vector of .txt filenames
     */
    std::vector<std::string> listTxtFilesInDirectory(const std::string &dir, int max);
}

#endif
