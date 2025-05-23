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

