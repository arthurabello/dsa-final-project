#import "@preview/ctheorems:1.1.3": *
#import "@preview/plotst:0.2.0": *
#import "@preview/codly:1.2.0": *
#import "@preview/codly-languages:0.1.1": *
#codly(languages: codly-languages)

#show: codly-init.with()
#show: thmrules.with(qed-symbol: $square$)
#show link: underline

#set heading(numbering: "1.1.")
#set page(numbering: "1")
#set heading(numbering: "1.")
#set math.equation(
  numbering: "1",
  supplement: none,
)
#show ref: it => {
  // provide custom reference for equations
  if it.element != none and it.element.func() == math.equation {
    // optional: wrap inside link, so whole label is linked
    link(it.target)[eq.~(#it)]
  } else {
    it
  }
}

#let theorem = thmbox("theorem", "Theorem", fill: rgb("#ffeeee")) //theorem color
#let corollary = thmplain(
  "corollary",
  "Corollary",
  base: "theorem",
  titlefmt: strong
)
#let definition = thmbox("definition", "Definition", inset: (x: 1.2em, top: 1em))
#let example = thmplain("example", "Example").with(numbering: "1.")
#let proof = thmproof("proof", "Proof")

//shortcuts


#align(right, text(12pt)[
  *FGV/EMAp*\
])


#align(center, text(17pt)[
  *Final Project - Inverted Index and Comparative Analysis*
])

#align(center, text(11pt)[
  Arthur Rabello Oliveira #footnote[#link("https://emap.fgv.br/")[Escola de Matemática Aplicada, Fundação Getúlio Vargas (FGV/EMAp)], email: #link("mailto:arthur.oliveira.1@fgv.edu.br")], Gabrielle Mascarello, Eliane Moreira, Nícolas Spaniol, Gabriel Carneiro
])
#set par(first-line-indent: 1.5em,justify: true)

#align(center)[
  *Abstract*\
  We present the implementation and comparative analysis of three fundamental data structures for indexing and searching textual documents: the Binary Search Tree (BST), the AVL Tree, and the Red-Black Tree (RBT). Each structure was implemented with its core operations, including insertion and search. Unit tests were developed to validate the correctness and performance of these implementations. We also provide a further comprehensive comparative study of the three trees based on their time complexity, balancing efficiency, and suitability for document indexing. The results demonstrate the trade-offs between implementation complexity and query performance, offering insights into the practical considerations for choosing appropriate search tree structures in information retrieval systems.
]

#outline()

#pagebreak()

= Introduction
<section_introduction>

== Motivation
<section_motivation>

== Problem Statement
<section_problem_statement>

== Overview of Search Trees
<section_overview_search_trees>


= Data Structures Overview
<section_data_structures_overview>

== Binary Search Tree (BST)
<section_bst_definitions>

== AVL Tree
<section_avl_definitions>

== Red-Black Tree (RBT)
<section_rbt_definitions>

== Inverted Index
<section_inverted_index_definitions>

== Comparison of Properties
<section_comparison_properties>

= Implementations
<section_implementations>

== Binary Search Tree (BST)
<section_bst_implementation>

=== Algorithms
<section_bst_algorithms>

=== Complextity Analysis
<section_bst_complexity_analysis>

== AVL Tree
<section_avl_implementation>

=== Algorithms

<section_avl_algorithms>

=== Complextity Analysis
<section_avl_complexity_analysis>

== Red-Black Tree (RBT)
<section_rbt_impementation>

=== Algorithms
<section_rbt_algorithms>

=== Complextity Analysis
<section_rbt_complexity_analysis>

== Inverted Index
<section_inverted_index_implementation>

=== Algorithms
<section_inverted_index_algorithms>

=== Complextity Analysis
<section_inverted_index_complexity_analysis>

= Testing and Validation
<section_testing_validation>

== Unit Testing Method
<section_unit_testing_method>

=== Binary Search Tree (BST)
<section_bst_testing>

=== AVL Tree
<section_avl_testing>

=== Red-Black Tree (RBT)
<section_rbt_testing>

= Comparative Analysis
<section_comparative_analysis>

== The Experiment
<section_the_experiment>

== Memory Usage
<section_memory_usage>

== Time Complexity
<section_time_complexity>

= Conclusion
<section_conclusion>

== Summary of Findings
<section_summary_findings>


= Source code
<section_source_code_repository>
(repository)

= Task Division  (Required by the professor)
<section_task_division>
== Arthur Rabello Oliveira
<section_task_division_arthur>

Implemented and documented the following functions:

```cpp
BinaryTree* createTree(){ //artu
        BinaryTree* newBinaryTree = new BinaryTree{nullptr};
        return newBinaryTree;
    }

SearchResult search(BinaryTree* binary_tree, const std::string& word) { //artu
        auto start_time = std::chrono::high_resolution_clock::now(); //start measuring time
        
        if (binary_tree == nullptr || binary_tree->root == nullptr) {
            auto end_time = std::chrono::high_resolution_clock::now(); //done lol
            double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;
            return {0, {}, duration, 0};
            
        } else {
            Node* current_node = binary_tree->root;
            int number_of_comparisons = 0;
            
            while (current_node != nullptr) {
                number_of_comparisons++;
                
                int compareResult = strcmp(word.c_str(), current_node->word.c_str());
                
                if (compareResult == 0) { //found!
                    auto end_time = std::chrono::high_resolution_clock::now();
                    double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;
                    return {1, current_node->documentIds, duration, number_of_comparisons};

                } else if (compareResult < 0) {
                    current_node = current_node->left; //go left because word is smaller
                } else {
                    current_node = current_node->right; //go right because word is bigger
                }
            }
            
            //if word not found
            auto end_time = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;
            return {0, {}, duration, number_of_comparisons};
        }
    }
```

== Gabrielle Mascarello
<section_task_division_gabrielle>

== Eliane Moreira
<section_task_division_eliane>

Implemented and documented the following functions:

```cpp
InsertResult insert(BinaryTree* binary_tree, const std::string& word, int documentId){ //eliane
        InsertResult result;
        int comparisons = 0;
        auto start_time = std::chrono::high_resolution_clock::now();

        Node* newNode = nullptr;
        
        if(binary_tree->root == nullptr){
            newNode = createNode(word, {documentId});
            binary_tree->root = newNode;
        } else {
            Node* current = binary_tree->root;
            Node* parent = nullptr;

            while (current != nullptr){
                parent = current;
                comparisons++;

                if(word == current->word){
                    //checks if documentId has already been added
                    bool found = false;
                    for(size_t i = 0; i < current->documentIds.size(); i++){
                        if (current->documentIds[i] == documentId) {
                            found = true;
                            break;
                        }
                    }
                    
                    if (found == false) {
                        current->documentIds.push_back(documentId);
                    }

                    auto end_time = std::chrono::high_resolution_clock::now();
                    double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;
    
                    result.numComparisons = comparisons;
                    result.executionTime = duration;
                    return result;

                } else if(word < current->word){
                    current = current->left;
                } else {
                    current = current->right;
                }
            }

            newNode = createNode(word, {documentId});
            newNode->parent = parent;

            if(word < parent->word){
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0;

        result.numComparisons = comparisons;
        result.executionTime = duration;
        return result; 
    }
```

== Nícolas Spaniol
<section_task_division_nicolas>

== Gabriel Carneiro
<section_task_division_gabriel>

Implemented and documented the following functions:

```cpp
Node* createNode(std::string word, std::vector<int>documentIds, int color = 0) { //sets for 0 if it the tree doesnt support red-black, gabriel carneiro
        
        Node* newNode = new Node;
        newNode->word = word;
        newNode->documentIds = documentIds;
        newNode->parent = nullptr;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1; //height of a new node is 1
        newNode->isRed = color; //0 for red, 1 for black
        return newNode;
    }

    void deleteBinaryTree(BinaryTree* binary_tree){ //gabriel carneiro
        Node* root = binary_tree->root;
        
        if(root != nullptr){
            Node* leftNode = root->left;
            BinaryTree* leftSubTree = createTree();
            leftSubTree->root = leftNode;
            
            Node* rightNode = root->right;
            BinaryTree* rightSubTree = createTree();
            rightSubTree->root = rightNode;
            
            delete root;

            deleteBinaryTree(leftSubTree);
            deleteBinaryTree(rightSubTree);

            delete binary_tree;
        }
    }
```


