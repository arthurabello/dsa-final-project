#import "@preview/ctheorems:1.1.3": *
#import "@preview/plotst:0.2.0": *
#import "@preview/codly:1.2.0": *
#import "@preview/codly-languages:0.1.1": *
#codly(languages: codly-languages)

#show: codly-init.with()
#show: thmrules.with(qed-symbol: $square$)  
#show link: underline
#show ref: underline

#set heading(numbering: "1.1.")
#set page(numbering: "1")
#set heading(numbering: "1.")
#set math.equation(
  numbering: "(1)",
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
  Arthur Rabello Oliveira #footnote[#link("https://emap.fgv.br/")[Escola de Matemática Aplicada, Fundação Getúlio Vargas (FGV/EMAp)], email: #link("mailto:arthur.oliveira.1@fgv.edu.br")], Gabrielle Mascarelo, Eliane Moreira, Nícolas Spaniol, Gabriel Carneiro
])
#set par(first-line-indent: 1.5em,justify: true)

#align(center)[
  *Abstract*\
  We present the implementation and comparative analysis of three fundamental data structures for indexing and searching textual documents: the Classic Binary Search Tree (BST), the AVL Tree, and the Red-Black Tree (RBT). Each structure was implemented with its core operations, including insertion and search. Unit tests were developed to validate the correctness and performance of these implementations. We also provide a further comprehensive comparative study of the three trees based on their time complexity, balancing efficiency, and suitability for document indexing. The results demonstrate the trade-offs between implementation complexity and query performance, offering insights into the practical considerations for choosing appropriate search tree structures in information retrieval systems.
]

#outline()

#pagebreak()

= Introduction
<section_introduction>

== Context
<section_context>

Humanity now produces more text in a single day than it did in the first two millennia of writing combined. Search engines must index billions of web pages, e-commerce sites hundreds of millions of product descriptions, and DevOps teams terabytes of log lines every hour. Scanning those datasets sequentially would be orders of magnitude too slow; instead, virtually all large-scale retrieval systems rely on an *inverted index*, a data structure that stores, for each distinct term, the identifiers of documents in which it appears.

== Problem Statement
<section_problem_statement>

While the logical view of an inverted index is a simple dictionary, its physical realisation must support two conflicting workloads:

- *Bulk ingestion* of millions of (term, docID) pairs per second.

- *Sub-millisecond* queries for ad-hoc keyword combinations.

Choosing the proper data structure is therefore a trade-off between build-time cost and implementation complexity.

== Objectives
<section_objectives>

+ *Implement* BST, AVL and Red-Black Tree insertion, deletion and search in C++.

+ *Build* an inverted index over a 10 k-document corpus with each tree.

+ *Measure* build time, query latency, and memory usage under identical workloads.

+ *Discuss* which structure best balances simplicity and performance for mid-scale information-retrieval tasks.

= Motivation
<section_motivation>

== Why Inverted Index?
<section_why_inverted_index>


#table(
  columns: 3,

  table.header(
    [*Domain*],
    [*Real-world system*],
    [*Role of the inverted index*],
  ),

  [Web search],            [Google, Bing, DuckDuckGo],          [Core term $->$ page mapping],
  [Enterprise search],     [Apache Lucene & Elasticsearch],     [Underlying index files and query engine],
  [Database systems],      [Postgres GIN & CockroachDB],        [Full-text and JSONB indexing],
  [Observability / Logs],  [Splunk, OpenObserve],               [Fast filtering / aggregation of terabyte-scale logs],
  [Bioinformatics],        [VariantStore, PAC],                 [Searching billions of DNA k-mers],
  [Operating systems],     [Linux schedulers & timers],         [Kernel subsystems use RBTs—conceptually an inverted index over time or PID keys],
)

These examples shows the ubiquity of inverted indexes in modern era. From web search engines to bioinformatics, inverted indexes are the backbone of efficient information retrieval systems.


= Definitions
<section_definitions>

== BST
<section_bst_definition>

In graph theory, a tree is a connected acyclic graph. A Binary Search Tree (BST) is a tree with the following properties:

+ Each node has at most two children, referred to as the left and right child.

+ The left child of a node contains only nodes with keys less than the node's key.

+ The right child of a node contains only nodes with keys greater than the node's key.


These properties are true $forall "node" in T = (V, E)$.

Here are some examples of BSTs:

#figure(
  image("images/example1_bst.png", width: 70%),
  caption: [
    Example of a Binary Search Tree
  ]
) <figure_example1_bst>

#figure(
  image("images/example2_bst.png", width: 50%),
  caption: [
    Another example of a Binary Search Tree
  ]
) <figure_example2_bst>

#figure(
  image("images/example3_bst.png", width: 50%),
  caption: [
    Degenerated Binary Search Tree
  ]
) <figure_example3_bst>


@figure_example1_bst and @figure_example2_bst are ordinary BST's, while @figure_example3_bst is a _degenerated_ BST. Which is a tree in which every node has at most one child, making it a linear chain of nodes. This is the worst case for a BST, due to computational complexity of some operations.

== AVL Tree
<section_avl_tree_definition>

The AVL tree can be seen as a solution to the degenerated BST problem. It is a self-balancing binary search tree, built so the heights of the two child subtrees of any node differ by at most one. This balance condition ensures that the tree remains approximately balanced, preventing the worst-case linear chain structure.

In order to maintain this balance rule, we define the balance factor of a node $n in V$ as:

$
  B_F (n) = h("RightSubtree"(n)) - h("LeftSubtree"(n))
$

If $B_F (n) < 0$, the tree is categorized as "left-heavy", if $B_F (n) > 0$, the tree is "right-heavy", and if $B_F (n) = 0$, the tree is "balanced". For the event $B_F (n) != 0$, _rotations_ are applied to restore balance.

Rotations are local tree restructuring operations that change the structure of the tree without violating the binary search tree property. There are $3$ types of rotations:

+ Left Rotation: Applied when a right-heavy subtree needs balancing.

+ Right Rotation: Applied when a left-heavy subtree needs balancing.

+ Permutations of Left and Right Rotations: Used to balance more complex imbalances.

#link("https://portaldoprofessor.fct.unesp.br/projetos/cadilag/apps/structs/arv_avl.php")[This] is a very good website to better visualize the creation of a tree. We recommend the user to play with it, inserting and deleting nodes, to see how the tree is balanced.

The AVL Tree was created by Georgy Adelson-Velsky and Evgenii Landis in 1962, and it was the first self-balancing binary search tree. The name "AVL" comes from the initials of their last names. More information can be found #link("https://en.wikipedia.org/wiki/AVL_tree")[here]. Below is an example of the creation of an AVL tree:

#figure(
  image("images/example_avl.gif", width: 70%),
  caption: [
    Creation of an AVL Tree
  ]
) <figure_example_avl_gif>


== RBT Tree
<section_rbt_tree_definition>

The RBT is another self-balancing BST, which uses a different balancing strategy than the AVL. RBT stands for Red-Black Tree. The Red-Black stands for the additional propery that each node is colored either red or black, and the tree satisfies the following properties:

+ The root node is always black.

+ Every leaf node is black.

+ If a node is red, then both of its children must be black (no two red nodes can be adjacent).

+ Every path from a node to its descendant leaf nodes must have the same number of black nodes (black-height).

Properties $3$ and $4$ forces long paths to pick up extra black nodes, capping the tree's height. Here is an example:

#figure(
  image("images/example_rbt.png", width: 70%),
  caption: [
    Example of a Red-Black Tree
  ]
) <figure_example_rbt>


= Implementations
<section_implementations>
== Binary Search Tree (BST)
<section_bst_implementation>

Since the AVL and RBT trees are both subsystems of the classic BST, we have implemented the classic BST in the #link("https://github.com/arthurabello/dsa-final-project/blob/main/src/tree_utils.cpp")[`tree_utils`] module, later used as based for the AVL and BST

=== Algorithms
<section_bst_algorithms>

#link("https://github.com/arthurabello/dsa-final-project/blob/main/src/tree_utils.cpp")[Here] are all the functions written for the classic BST, and #link("https://github.com/arthurabello/dsa-final-project/blob/main/src/tree_utils.h")[here] is the header file with the corresponding documentation. The list of functions is:

- `createNode`

- `createTree`

- `search`

- `deletionPostOrder`

- `destroy`

- `calculateHeight`

- `updateHeightUp`

=== Complexity Analysis
<section_bst_complexity_analysis>

Below is a full complexity analysis:

*`createNode`*:

Clearly $O(1)$, the function allocates memory for a new node and initializes it, independent of the size of the tree.

*`createTree`*:

Also $O(1)$, an empty BST is allocated and initialized with a `nullptr` root.

*`search`*:

The search operation, unavoidably, has a time complexity of $O(h)$, with $h$ being the height of the tree. The function follows a single root-to-leaf path in the BST, making at most $h$ comparisons. No recursion, only a few local variables.

*`deletionPostOrder`*:

This function is $O(phi)$, where $phi$ is the size of the subtree rooted at the node to be deleted. It is a classic post-order traversal: each node is visited & deleted precisely once.

*`destroy`*:

This functon simply calls `deletionPostOrder` on the root node, so it is $O(h)$.

*`calculateHeight`*:

This is $O(k)$, with $k = "subtree size"$. It 	recursively explores both children of every node once to compute `max(left,right)+1`.

*`updateHeightUp`*:

This function is $O(h)$. It iterates upward, recomputing height until it stops changing or reaches the root; at most $h$ ancestor steps, no recursion on children. 

== AVL Tree
<section_avl_implementation>
=== Algorithms
<section_avl_algorithms>

The functions implemented _stritly_ for the AVL can be found #link("https://github.com/arthurabello/dsa-final-project/blob/main/src/avl/avl.cpp")[here], and the header file with the corresponding documentation #link("https://github.com/arthurabello/dsa-final-project/blob/main/src/avl/avl.h")[here]. We have used many of the BST functions, as previously stated. The list of AVL-functions is:

- `getHeight(Node*)`

- `bf(Node*)` [balance factor]

- `leftRotation(BinaryTree&, Node*)`

- `rightRotation(BinaryTree&, Node*)`

- `insert(BinaryTree& binary_tree, const std::string& word, int documentId)`

- `remove(Node*&, key)`

- `remove(BinaryTree&, key)`

- `updateHeightUp(Node*)`

- `clear(BinaryTree*)`

- `printInOrder`

- `printLevelOrder`

=== Complexity Analysis
<section_avl_complexity_analysis>

Below is a full complexity analysis of the AVL functions:

*`getHeight`*:

$O(1)$, it simply returns the stored `height` of a node.

*`bf(Node*)`*:

Also $O(1)$, this is nothing more than a subtraction of two integers.

*`leftRotation(BinaryTree&, Node*)`*:

This is a constant quantity of pointer rewires and some height updates, so $O(1)$.

*`rightRotation(BinaryTree&, Node*)`*:

Same as above, $O(1)$.

*`insert(BinaryTree& binary_tree, const std::string& word, int documentId)`*:

This is $O(h)$, where $h$ is the height of the tree. It does one BST descent $O(h)$ at most one rebalance per level. The rotations are constant.

*`remove(Node*&, key)`*:

This is $O(h)$. It is a classic BST deletion + at most one structural deletion + up-to-root rebalancing.

*`remove(BinaryTree&, key)`*:

Same as above, $O(h)$.

*`updateHeightUp(Node*)`*:

This function iterates upward recomputing height until the value stabilises or the root is reached. Therefore it is $O(h)$.

*`clear(BinaryTree*)`*:

Clearly $O(n)$, where $n$ is the size of the tree. It removes each node once.

*`printInOrder`*:

This is a classic traversal visiting every node once, so it is $O(n)$.

*`printLevelOrder`*:

Same as above, $O(n)$.

== Red-Black Tree (RBT)
<section_rbt_impementation>
=== Algorithms
<section_rbt_algorithms>

The functions implemented _strictly_ for the RBT can be found #link("https://github.com/arthurabello/dsa-final-project/blob/main/src/rbt/rbt.cpp")[here], and the header file with the corresponding documentation #link("https://github.com/arthurabello/dsa-final-project/blob/main/src/rbt/rbt.h")[here]. We have used many of the BST functions, as previously stated. The list of RBT-functions is:

- `insert(BinaryTree& binary_tree, const std::string& word, int documentId)`

- `int fixInsert(Node** root, Node* z)`

- `Node* getUncle(Node* node)`

- `Node* getSibling(Node* node)`

=== Complexity Analysis
<section_rbt_complexity_analysis>

Below is a full complexity analysis of the RBT functions:

*`int fixInsert(Node** root, Node* z)`*:

The while loop climbs the tree one level per iteration, recolouring and performig $<= 2$ rotations per level. Since the maximum height of an RBT is $O(log n)$@uwi_website, this is $O(log n)$.

*`insert(BinaryTree& t, const std::string& word, int documentId)`*:

This is a standard descent to find the leaf position, and $"height" <= O(log n)$, then it calls `fixInsert`, so the total complexity is also $O(log n)$.

*`Node* getUncle(Node* node)`*:

There is no loop or recursion, just a few pointer dereferences, so this is $O(1)$.

*`getSibling(Node* node)`*:

Same reasoning as above, $O(1)$.

== Inverted Index
<section_inverted_index_implementation>
=== Algorithms
<section_inverted_index_algorithms>

=== Complexity Analysis
<section_inverted_index_complexity_analysis>

= Testing and Validation
<section_testing_validation>
== Unit Testing Method
<section_unit_testing_method>

We have used the #link("https://github.com/ThrowTheSwitch/Unity/tree/b9d897b5f38674248c86fb58342b87cb6006fe1f")[*Unity*] submodule for unit testing (espanio faz teu nome)

All trees were equivalently tested under the same principles blablabla

The testing module can be found here blablabla

= Visualization with JavaScript
<section_visualization>

espanio faz teu nome


= Comparative Analysis
<section_comparative_analysis>

Here we develop a full comparative analysis of the three implemented data structures. We will use the following metrics:

+ Memory usage
+ Insertion time
+ Search time


== The Experiment
<section_the_experiment>

For the graphs seen in @section_graphs, we have used the following:

```bash
./[tree_name] stats 1000 data
```

All trees were testes with the same first 1000 .txt file, which can be found #link("https://github.com/arthurabello/dsa-final-project/blob/main/data/data.zip")[here].



== Graphs
<section_graphs>

For a better visualization of the statistics we will cover here, we recommend, through #link("https://github.com/arthurabello/dsa-final-project")[the repository], to run:

```bash
make bst && make avl && make rbt
./(tree_name) stats 1000 data
```

Opening the generated JavaScript visualization in a browser, you will be able to see the graphs and the tree visualizations in a more interactive way.

All statistics are also available in the `(tree_name).csv` file, generated by the `stats` command.

=== AVL
<section_graphs_avl>

These graphs were generated by thye JavaScript visualizer.

#figure(
  image("images/avl_comparison.png", width: 70%),
  caption: [
    Number of Comparisons done by the AVL Tree
  ] 
) <figure_avl_comparison>

#figure(
  image("images/avl_insertion.png", width: 70%),
  caption: [
    Insertion time of the AVL Tree
  ]
) <figure_avl_insertion>

#figure(
  image("images/avl_search.png", width: 70%),
  caption: [
    Search time of the AVL Tree
  ]
) <figure_avl_search>

=== BST
<section_graphs_bst>

These graphs were generated by thye JavaScript visualizer.

#figure(
  image("images/bst_comparisons.png", width: 70%),
  caption: [
    Number of Comparisons done by the BST Tree
  ]
) <figure_bst_comparisons>

#figure(
  image("images/bst_insertion.png", width: 70%),
  caption: [
    Insertion time of the BST Tree
  ]
) <figure_bst_insertion>

#figure(
  image("images/bst_search.png", width: 70%),
  caption: [
    Search time of the BST Tree
  ]
) <figure_bst_search>

=== RBT
<section_graphs_rbt>

These graphs were generated by thye JavaScript visualizer.


= Conclusion
<section_conclusion>

== Raw stats
<section_raw_stats>

The raw statistics for the trees are:

#table(
  columns: 4,
  inset: 10pt,
  align: horizon,

  table.header(
    [*Metric*], [*BST*], [*AVL*], [*RBT*],
  ),

  [Docs indexed],               [1000],      [1000],      [0],
  [Words indexed],              [213099],    [213099],    [0],
  [Total insertion time (ns)],  [373834912], [393149216], [0],
  [Avg insertion time (ns)],    [1754],      [1844],      [0],
  [Max insertion time (ns)],    [124644],    [189938],    [0],
  [Min insertion time (ns)],    [168],       [147],       [0],
  [Total search time (ns)],     [90034],     [81607],     [0],
  [Avg search time (ns)],       [365],       [331],       [0],
  [Max search time (ns)],       [910],       [773],       [0],
  [Min search time (ns)],       [243],       [232],       [0],
  [Total comparisons (search)], [3356],      [2942],      [0],
  [Avg comparisons (search)],   [13],        [11],        [0],
  [Max comparisons (search)],   [25],        [16],        [0],
  [Min comparisons (search)],   [1],         [2],         [0],
  [Node count],                 [16986],     [16986],     [0],
  [Tree height],                [32],        [16],        [0],
  [Min depth],                  [4],         [10],        [0],
  [Balance diff],               [28],        [6],         [0],
  [Relative balance],           [8],         [1.6],       [0],
)

== Actual Analysis
<section_actual_analysis>

Let $h(n)$ be the height of node $n$. Because every AVL is a BST with an additional constraint, every theorem about BST search order also applies to AVL unless it contradicts the balance rule.

For the BST, If the input keys arrive in sorted order, each new key is inserted as the right child of the previous one, yielding a linear chain of *n* nodes, so:

$
  h_(max) (n) = n - 1
$

For the AVL, the maximum height is logarithmical @mit_website, as it is a balanced tree. The height of an AVL tree with *n* nodes is at most:

$
  h_(max) (n) = O(log n)
$

Which might make the AVL interesting.

=== Time Complexity
<section_time_complexity>

When analyzing time complexity for these trees, we look at the cost of each operation:

#table(
  columns: 4,
  align: horizon,

  table.header(
    [*Operation*], [*BST*], [*AVL*], [*Proof Idea*],
  ),

  [Insertion], [$O(h(n))$], [$O(h) + O(1) "rotations"$], [Keys are compared on a root-to-leaf path, so the height of the tree determines the number of comparisons. For BST, this is linear in the worst case, while AVL guarantees logarithmic height due to balancing rules.],

  [Search],    [$O(h(n))$], [$O(h(n))$], [Shown in @section_avl_complexity_analysis and @section_bst_complexity_analysis],
  [Deletion],  [$O(h(n))$], [$O(h) + O(h) "rotations worst-case"$], [Each descent to find the node to delete is $O(h)$, and rebalancing may require up to $O(h)$ rotations in the worst case. For BST, this is linear in the worst case, while AVL guarantees logarithmic height due to balancing rules.],
)

=== Memory Usage
<section_memory_usage>

All implementations use:

```cpp
struct Node {
    std::string word;
    std::vector<int> docIds;
    Node *left, *right, *parent;
    int height;        // both codes already keep this
    bool isRed;        // reserved for RBT
};
```

Field counts are identical, therefore heap consumption is $O(n)$ for the AVL and BST. Recursive algorithms allocate one activation record per visited level.

- BST worst-case stack depth: $h = n - 1 -> O(n)$ extra bytes; may overflow for large $n$.

- AVL stack depth: $h = O(log n) -> $ asymptotically optimal.

No additional global buffers are required; rotations operate with $O(1)$ local variables.

= Challenges and Difficulties (Required by the professor)
<section_challenges_difficulties>

== Arthur Rabello Oliveira
<section_challenges_difficulties_arthur>

The hardest challenge i faced was to keep the GitHub repository organized, reviewing pull requests from my fellow collaborators. Writing this report and the `README.md` have not been trivial tasks too. 

Another very big difficulty was to write the `Makefile` for compiling the project, as the syntax is rather counterintuitive. One could scroll through the commit history and get a glance at how ugly the first version of this file was.

Reviewing the work of my fellow collaborators was also a challenge, as I had to understand their code and suggest improvements. This was a very good learning experience, though.

I must say that with other 4 courses, time management was a *very big challenge* too. Not only 4 other courses, but 4 other *hard* courses, with many assignments and deadly exams and tests, but unfortunately my days have not more than 24 hours, so I had to manage my time very carefully to be able to finish this project on time. And i am very happy with the result.

== Eliane Moreira
<section_challenges_difficulties_eliane>

== Gabrielle Mascarelo
<section_challenges_difficulties_gabrielle>

== Nícolas Spaniol
<section_challenges_difficulties_nicolas>

== Gabriel Carneiro
<section_challenges_difficulties_gabriel>

= Source code
<section_source_code_repository>

All implementations and tests are available in the public repository at https://github.com/arthurabello/dsa-final-project

= Task Division  (Required by the professor)
<section_task_division>
== Arthur Rabello Oliveira
<section_task_division_arthur>

Contributed with:

- Keeping the repository civilized (main-protecting rules, enforcing code reviews)

- Writing and documenting the `Makefile` for building the project

- Writing the `README.md` and `report.typ`

- Writing and documenting functions for the classic BST in `bst.cpp`

== Gabrielle Mascarelo
<section_task_division_gabrielle>

Contributed with:

- Writing and documenting functions to read files in `data.cpp`

- Structuring statistics in the CLI 

== Eliane Moreira
<section_task_division_eliane>

Contributed with:

- Testing all function related to the BST

- Writing and documenting functions for `tree_utils.cpp`

- Fixing bugs in `data.h`

== Nícolas Spaniol
<section_task_division_nicolas>

Contributed with:

- Code reviews and suggestions for improvements in the codebase.

- Built from scratch the JavaScript visualization of all trees

== Gabriel Carneiro
<section_task_division_gabriel>

Contributed with:

- Writing and documenting functions for the classic BST

- Writing and documenting functions for the AVL Tree

- Testing functions for `tree_utils.cpp`

#bibliography("bibliography.bib")

