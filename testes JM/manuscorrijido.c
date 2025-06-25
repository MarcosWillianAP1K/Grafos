#include <stdio.h>
#include <stdlib.h>

// Estrutura para representar um grafo
typedef struct Grafo {
    int numVertices;
    int** matrizAdj; // Matriz de adjacência para armazenar arestas
} Grafo;

// Função para criar um novo grafo com n vértices
Grafo* criarGrafo(int n) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = n;
    grafo->matrizAdj = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        grafo->matrizAdj[i] = (int*)calloc(n, sizeof(int));
    }
    return grafo;
}

// Função para liberar a memória do grafo
void liberarGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        free(grafo->matrizAdj[i]);
    }
    free(grafo->matrizAdj);
    free(grafo);
}

// Função para adicionar uma aresta ao grafo
void adicionarAresta(Grafo* grafo, int v, int w) {
    grafo->matrizAdj[v][w] = 1;
    grafo->matrizAdj[w][v] = 1; // Grafo não direcionado
}

// Função para criar um grafo ciclo C6 (6 vértices)
Grafo* criarC6() {
    Grafo* c6 = criarGrafo(6);
    for (int i = 0; i < 6; i++) {
        adicionarAresta(c6, i, (i + 1) % 6);
    }
    return c6;
}

// Função para obter um emparelhamento perfeito de C6
void obterEmparelhamentoPerfeito(int emparelhamento[3][2]) {
    emparelhamento[0][0] = 0; emparelhamento[0][1] = 1;
    emparelhamento[1][0] = 2; emparelhamento[1][1] = 3;
    emparelhamento[2][0] = 4; emparelhamento[2][1] = 5;
}

// Função para realizar a operação de enxerto
Grafo* enxertar(Grafo* Q, int v, int w, Grafo* G, int ab[2]) {
    int novoNumVertices = G->numVertices - 2 + Q->numVertices;
    Grafo* R = criarGrafo(novoNumVertices);

    // Copiar arestas de Q para R
    for (int i = 0; i < Q->numVertices; i++) {
        for (int j = 0; j < Q->numVertices; j++) {
            if (Q->matrizAdj[i][j]) {
                R->matrizAdj[i][j] = 1;
            }
        }
    }

    // Copiar arestas de G para R, excluindo vértices a e b
    int a = ab[0], b = ab[1];
    int deslocamento = Q->numVertices;
    for (int i = 0; i < G->numVertices; i++) {
        if (i == a || i == b) continue;
        for (int j = 0; j < G->numVertices; j++) {
            if (j == a || j == b) continue;
            int novoI = (i < a) ? i : (i < b) ? i - 1 : i - 2;
            int novoJ = (j < a) ? j : (j < b) ? j - 1 : j - 2;
            if (G->matrizAdj[i][j]) {
                R->matrizAdj[deslocamento + novoI][deslocamento + novoJ] = 1;
            }
        }
    }

    // Conectar Q a G
    for (int i = 0; i < G->numVertices; i++) {
        if (i == a || i == b) continue;
        int novoI = (i < a) ? i : (i < b) ? i - 1 : i - 2;
        if (G->matrizAdj[a][i]) {
            R->matrizAdj[v][deslocamento + novoI] = 1;
            R->matrizAdj[deslocamento + novoI][v] = 1;
        }
        if (G->matrizAdj[b][i]) {
            R->matrizAdj[w][deslocamento + novoI] = 1;
            R->matrizAdj[deslocamento + novoI][w] = 1;
        }
    }

    return R;
}

// Função para imprimir a matriz de adjacência
void imprimirMatrizAdj(Grafo* grafo) {
    printf("\nMatriz de Adjacência do Grafo H_n (%d vértices):\n", grafo->numVertices);
    for (int i = 0; i < grafo->numVertices; i++) {
        for (int j = 0; j < grafo->numVertices; j++) {
            printf("%d ", grafo->matrizAdj[i][j]);
        }
        printf("\n");
    }
}

// Função recursiva para construir o grafo de Hanoi H_n
Grafo* construirGrafoHanoi(int n) {
    if (n == 1) {
        Grafo* H1 = criarGrafo(3); // H1 é um triângulo (C3)
        adicionarAresta(H1, 0, 1);
        adicionarAresta(H1, 1, 2);
        adicionarAresta(H1, 2, 0);
        return H1;
    }

    Grafo* H_n_menos_1 = construirGrafoHanoi(n - 1);
    Grafo* C6 = criarC6();
    int emparelhamento[3][2];
    obterEmparelhamentoPerfeito(emparelhamento);

    Grafo* H_n = C6;
    int v = 0, w = 1; // Assumindo vértices de grau 2 em H_{n-1}

    // Enxertar três cópias de H_{n-1} em C6
    for (int i = 0; i < 3; i++) {
        Grafo* temp = enxertar(H_n_menos_1, v, w, H_n, emparelhamento[i]);
        liberarGrafo(H_n);
        H_n = temp;
    }

    liberarGrafo(H_n_menos_1);
    return H_n;
}

// Função principal para teste
int main() {
    int n = 3; // Exemplo: construir H_3
    Grafo* H_n = construirGrafoHanoi(n);
    printf("Grafo de Hanoi H_%d construído com %d vértices\n", n, H_n->numVertices);
    imprimirMatrizAdj(H_n); // Imprimir a matriz de adjacência
    liberarGrafo(H_n);
    return 0;
}