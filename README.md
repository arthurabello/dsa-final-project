# Inverted Index & Search Tree Comparative Study

> Final project – FGV/EMAp – Data Structures & Algorithms *(2025)*

[![Build Status](https://img.shields.io/badge/status-WIP-lightgrey)](#)

A **work‑in‑progress** codebase that will implement and compare three classic search‑tree data structures—**BST**, **AVL**, and **Red‑Black Tree**—as back‑ends for an inverted index over textual documents.  The heavy theoretical details live in our 💼 `report.pdf`; 

---

## 📋 Table of Contents

1. [Project Overview](#project-overview)
2. [Prerequisites](#prerequisites)
3. [Usage](#usage)
4. [Branching & Workflow](#branching--workflow)
5. [Documentation & Report](#documentation--report)
6. [License](#license)
7. [Authors](#authors)

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

## Prerequisites

* **[C++](https://cpp-lang.net/)** compiler (we used g++ 15.1.1 20250425)

* **[Python](https://www.python.org/)** ≥ 3.10 (optional, for analysis)

---

## Cloning

Due to submodules usage, this repository must be cloned with:

```bash
git clone https://github.com/arthurabello/dsa_final_project.git --recursive
```

Please note the `--recursive` at the very end.

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

# RBT CLI
make build-avl
./rbt <search|stats|view> <n_docs> <directory>

# RBT Tests
make test-rbt
```

---

## Branching & Workflow

We follow a **trunk‑based with topics** strategy:

* `main` – stable.
* `feature/<scope>` – atomic features
* `test/<scope>` – dedicated test suites
* `documentation/<scope>` – documentation & report tweaks.

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
