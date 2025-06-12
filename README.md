# Inverted Index & Search Tree Comparative Study

> Final project – FGV/EMAp – Data Structures & Algorithms *(2025)*

[![Build Status](https://img.shields.io/badge/status-WIP-lightgrey)](#)

A **work‑in‑progress** codebase that will implement and compare three classic search‑tree data structures—**BST**, **AVL**, and **Red‑Black Tree**—as back‑ends for an inverted index over textual documents.  The heavy theoretical details live in our 💼 `report.pdf`; 

---

## 📋 Table of Contents

1. [Project Overview](#project-overview)
2. [Repository Structure](#repository-structure)
3. [Prerequisites](#prerequisites)
4. [Installation](#installation)
5. [Usage](#usage)
6. [CLI Reference](#cli-reference)
7. [Running the Experiments](#running-the-experiments)
8. [Testing](#testing)
9. [Benchmarking](#benchmarking)
10. [Branching & Workflow](#branching--workflow)
11. [Documentation & Report](#documentation--report)
12. [License](#license)

---

## Project Overview

Huge docs, even bigger trees.  We will:

* parse a corpus of text files into tokens;

* build an **inverted index** using three searching structures (BST/AVL/RBT);

* expose a minimal **command‑line interface** for indexing & search;

* collect timing / memory metrics and present a comparative analysis.

See [`report.pdf`](./report.pdf) for the full problem statement and planned methodology.

*This section will stay short on purpose.*

---

## Repository Structure

```
to be determined...
```

---

## Prerequisites

*Being worked on*

* **[C++](https://cpp-lang.net/)** compiler (we used g++ 15.1.1 20250425)

* **[Python](https://www.python.org/)** ≥ 3.10 (optional, for analysis notebooks)          *being worked on*

---

## Cloning

Due to submodules usage, this repository must be cloned with:

```bash
git clone https://github.com/arthurabello/dsa_final_project.git --recursive
```

Please note the `--recursive` at the very end.


## Installation

```bash
to be determined...
```

---

## Usage

```bash
# BST CLI
make build-bst
./bst <search|stats|view> <n_docs> <directory>

# BST Tests
make test-bst

# AVL CLI
make build-avl
./avl <search|stats|view> <n_docs> <directory>

# AVL Tests
make test-avl
```


---

## CLI Reference

*Being worked on*

| Command        | Description                     | Status |
| -------------- | ------------------------------- | ------ |
| `bst_index`    | Build BST‑backed inverted index | done  |
| `avl_index`    | Build AVL‑backed inverted index | *WIP*  |
| `rbt_index`    | Build RBT‑backed inverted index | *WIP*  |
| `query_<tree>` | Run boolean / ranked queries    | *WIP*  |

---

## Running the Experiments

*Being worked on*

---

## Testing

```bash
# Run tree utilities tests
g++ Unity/src/unity.c src/tree_utils.cpp src/test_tree_utils.cpp; ./a.out

# Run BST tests
g++ Unity/src/unity.c src/tree_utils.cpp src/bst/bst.cpp src/bst/test_bst.cpp; ./a.out

```

---

## Benchmarking

*Being worked on*

Performance harness powered by **matplotlib** plots.

---

## Branching & Workflow

We follow a **trunk‑based with topics** strategy:

* `main` – stable.
* `feature/<scope>` – atomic features
* `test/<scope>` – dedicated test suites
* `documentation/<scope>` – documentation & report tweaks.

> Pull requests must pass *all* checks before merging.  *Being worked on*

---

## Documentation & Report

Full theoretical background, design decisions, and result discussion live in `report.pdf`.  This README will **only** host the practical how‑tos.

---

## License

See the [LICENSE](./LICENSE) file for details.

---

## Authors

1. **[Arthur Rabello Oliveira](https://github.com/arthurabello)**
2. **[Eliane Moreira](https://github.com/ElianeMoreira)**
3. **[Gabriel Caneiro](https://github.com/gabcarn)**
4. **[Gabrielle Mascarelo](https://github.com/g4briwlle)**
5. **[Nícolas Spaniol](https://github.com/nicolasspaniol)**

*This README is intentionally skeletal – we will flesh out each section as development progresses*
