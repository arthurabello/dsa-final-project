import subprocess
from subprocess import DEVNULL
import csv
import matplotlib.pyplot as plt
import numpy as np


def plot(field, interval):
    """
    Draws the three-tree performance curve for a given metric.
    Adds a reference c·log₂n curve **only** when `field == "avgSearchTimeNs"`.
    """
    bst, avl, rbt = [], [], []

    for i in interval:
        subprocess.run(["./bst", "stats", str(i), "data"], stdout=DEVNULL)
        with open("bst.csv", newline="") as f:
            bst.append(float(next(csv.DictReader(f))[field]))

        subprocess.run(["./avl", "stats", str(i), "data"], stdout=DEVNULL)
        with open("avl.csv", newline="") as f:
            avl.append(float(next(csv.DictReader(f))[field]))

        subprocess.run(["./rbt", "stats", str(i), "data"], stdout=DEVNULL)
        with open("rbt.csv", newline="") as f:
            rbt.append(float(next(csv.DictReader(f))[field]))

        print(f"Done i = {i}")

    # curves for each tree
    plt.plot(interval, bst, label="BST")
    plt.plot(interval, avl, label="AVL")
    plt.plot(interval, rbt, label="RBT")

    # reference curve only for the search-time graph
    if field == "avgSearchTimeNs":
        x = np.asarray(interval, dtype=float)
        y = np.log2(x)
        c = bst[0] / y[0] if y[0] != 0 else 1.0
        plt.plot(x, c * y, "--", label=r"$c\log_2 n$")

    plt.legend()


# 1 ── Comparisons per search
plot("avgComparisonsSearch", [5, 10, 20, 50, 100, 200])
plt.title("Média de comparações na pesquisa de palavras por árvore e tamanho")
plt.xlabel("Número de documentos indexados")
plt.ylabel("Média de comparações")
plt.show()

# 2 ── Search-time plot (includes log-n reference)
plot("avgSearchTimeNs", [500, 1000, 2000, 4000, 8000])
plt.title("Média de tempo de pesquisa de palavras por árvore e tamanho")
plt.xlabel("Número de documentos indexados")
plt.ylabel("Média do tempo de pesquisa (nanossegundos)")
plt.show()

# 3 ── Insertion time
plot("avgInsertionTimeNs", [2 ** i for i in range(3, 11)])
plt.title("Média de tempo de inserção de palavras por árvore e tamanho")
plt.xlabel("Número de documentos indexados")
plt.ylabel("Média do tempo de inserção (nanossegundos)")
plt.show()

# 4 ── Relative balance ratio
plot("relativeBalance", [100, 250, 500, 750, 1000, 1500, 2000, 3000])
plt.title("Balanceamento relativo da árvore por tipo de árvore e tamanho")
plt.xlabel("Número de documentos indexados")
plt.ylabel("Balanceamento (altura máx. / altura mín.)")
plt.ylim(bottom=0)
plt.show()

# 5 ── Absolute balance difference
plot("balanceDiff", [10, 20, 40, 70, 100, 150, 300, 500, 750, 1000])
plt.title("Diferença no balanceamento da árvore por tipo de árvore e tamanho")
plt.xlabel("Número de documentos indexados")
plt.ylabel("Diferença (altura máx. - altura mín.)")
plt.ylim(bottom=0)
plt.show()
