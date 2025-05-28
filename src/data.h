#ifndef DATA_H
#define DATA_H
#include <iostream>
#include <vector>
#include <fstream>
#include "tree_utils.h"


namespace DATA {

    static std::string normalise(std::string w);
        /**
     * @brief Converts the string passed as argument
     * in standard ascii format, i.e all letters in
     * lowercase and without accents.
     * 
     * @param w Word you wish to normalise.buildIndex
     * @return Normalised argument
     */
    
    std::vector<std::string> tokenize(std::string filename);
    /**
     * @brief Given a filepath, opens the file and stores the
     * words in it on a vector.
     * 
     * @param filename the path of the archive.
     * 
     * @return vector with all the words in the given archive.
     * 
     */

    /**
    * @brief Retrieves a list of all .txt filenames from a specified directory.
    *
    * This function scans the given directory path and returns a vector
    * containing the names (not full paths) of all files ending with the ".txt" extension.
    *
    * @param directory_path The path to the directory to scan.
    * @return std::vector<std::string> A list of .txt filenames.
    * Returns an empty vector if the directory cannot be accessed or no .txt files are found.
    */
    std::vector<std::string> list_files_txt_in_path(const std::string &dir_path);

    /**
    * @brief Reads the entire content of a specified file into a string.
    *
    * @param full_file_path The complete path (including filename) to the file to be read.
    * @return std::string The content of the file.
    * Returns an empty string if the file cannot be opened or read.
    */
    std::string read_file_content(const std::string& full_file_path);
}
#endif