import subprocess
from subprocess import DEVNULL
import csv
import matplotlib.pyplot as plt


def plot(field, interval):
    bst = []
    avl = []
    rbt = []

    for i in interval:
        subprocess.run(["./bst", "stats", str(i), "data"], stdout=DEVNULL) 
    
        with open("bst.csv", newline="") as f:
            data = list(csv.DictReader(f))
            bst.append(float(data[0][field]))
    
        subprocess.run(["./avl", "stats", str(i), "data"], stdout=DEVNULL) 
    
        with open("avl.csv", newline="") as f:
            data = list(csv.DictReader(f))
            avl.append(float(data[0][field]))
    
        subprocess.run(["./rbt", "stats", str(i), "data"], stdout=DEVNULL) 
    
        with open("rbt.csv", newline="") as f:
            data = list(csv.DictReader(f))
            rbt.append(float(data[0][field]))
    
        print(f"Done i = {i}")
    
    plt.plot(interval, bst, label="BST")
    plt.plot(interval, avl, label="AVL")
    plt.plot(interval, rbt, label="RBT")
    plt.legend()


plot("avgComparisonsSearch", [5, 10, 20, 50, 100, 200])
plt.title("Média de comparações na pesquisa de palavras por árvore e tamanho")
plt.xlabel("Número de documentos indexados")
plt.ylabel("Média de comparações")
plt.show()

plot("avgSearchTimeNs", [500, 1000, 2000, 4000, 8000])
plt.title("Média de tempo de pesquisa de palavras por árvore e tamanho")
plt.xlabel("Número de documentos indexados")
plt.ylabel("Média do tempo de pesquisa (nanossegundos)")
plt.show()

plot("avgInsertionTimeNs", [int(2 ** i) for i in range(3, 11)])
plt.title("Média de tempo de inserção de palavras por árvore e tamanho")
plt.xlabel("Número de documentos indexados")
plt.ylabel("Média do tempo de inserção (nanossegundos)")
plt.show()

plot("relativeBalance", [100, 250, 500, 750, 1000, 1500, 2000, 3000])
plt.title("Balanceamento relativo da árvore por tipo de árvore e tamanho")
plt.xlabel("Número de documentos indexados")
plt.ylabel("Balanceamento (altura máx. / altura mín.)")
plt.ylim(bottom=0)
plt.show()

plot("balanceDiff", [10, 20, 40, 70, 100, 150, 300, 500, 750, 1000])
plt.title("Diferença no balanceamento da árvore por tipo de árvore e tamanho")
plt.xlabel("Número de documentos indexados")
plt.ylabel("Diferença (altura máx. - altura mín.)")
plt.ylim(bottom=0)
plt.show()
