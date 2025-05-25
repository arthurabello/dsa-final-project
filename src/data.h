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
     * @returns Normalised argument
     */
    
    
    std::vector<std::string> tokenize(std::string filename);
    /**
     * 
     * 
     * 
     * 
     */

    void buildIndex(const std::string& rootFolder, TREE::BinaryTree* tree);

    std::vector<TREE::Node*> buildNodes(std::string filename);
}

#endif