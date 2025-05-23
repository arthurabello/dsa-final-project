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

* **[C++](https://cpp-lang.net/)** compiler  *being worked on*

* **[Python](https://www.python.org/)** ≥ 3.10 (optional, for analysis notebooks)          *being worked on*

---

## Installation

```bash
to be determined...
```

---

## Usage

```bash
to be determined...
```


---

## CLI Reference

*Being worked on*

| Command        | Description                     | Status |
| -------------- | ------------------------------- | ------ |
| `bst_index`    | Build BST‑backed inverted index | *WIP*  |
| `avl_index`    | Build AVL‑backed inverted index | *WIP*  |
| `rbt_index`    | Build RBT‑backed inverted index | *WIP*  |
| `query_<tree>` | Run boolean / ranked queries    | *WIP*  |

---

## Running the Experiments

*Being worked on*

---

## Testing

*Being worked on*

```bash
to be determined...
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
2. **[Gabriel Caneiro](https://github.com/gabcarn)**
3. **[Gabrielle MAscarello](https://github.com/g4briwlle)**
4. **[Nícolas Spaniol](https://github.com/nicolasspaniol)**
5. **[Eliane Moreira](https://github.com/ElianeMoreira)**

*This README is intentionally skeletal – we will flesh out each section as development progresses*
