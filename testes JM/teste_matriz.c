#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 2       // Número de discos (reduzido para teste)
#define Pinos 3
#define MAX 9 // 3^2 = 9 configurações possíveis

typedef struct {
    int conf[N];
} vertice;

int adj[MAX][MAX];

//-----------------------------------------------
void configuracao(vertice *grafo) {
    for (int i = 0; i < MAX; ++i) {
        int num = i;
        for (int j = 0; j < N; ++j) {
            grafo[i].conf[j] = num % Pinos + 1;
            num /= Pinos;
        }
    }
}

//-----------------------------------------------
int aresta(vertice v1, vertice v2) {
    int diff = 0, disco_alterado = -1;

    for (int i = 0; i < N && diff <= 1; ++i) {
        if (v1.conf[i] != v2.conf[i]) {
            diff++;
            disco_alterado = i;
        }
    }

    // Se mais de um disco foi alterado, não é movimento válido
    if (diff > 1) return 0;
    
    // Se nenhum disco foi alterado, é o mesmo vértice
    if (diff == 0) return 0;

    int invalido = 1;

    // Verifica se o movimento é válido (não há disco menor em cima do disco movido)
    for (int i = 0; i < N && invalido; ++i) {
        if (i != disco_alterado && 
            ((v1.conf[i] == v1.conf[disco_alterado] && i < disco_alterado) || 
             (v2.conf[i] == v2.conf[disco_alterado] && i < disco_alterado))) {
            invalido = 0;
        }
    }

    return invalido;
}

//-----------------------------------------------
void adjacencia(vertice *grafo) {
    for (int i = 0; i < MAX; ++i)
        for (int j = 0; j < MAX; ++j)
            adj[i][j] = aresta(grafo[i], grafo[j]) ? 1 : 0;
}

//-----------------------------------------------
void imprimirConfiguracoes(vertice *grafo) {
    printf("Configuracoes dos vertices:\n");
    for (int i = 0; i < MAX; ++i) {
        printf("Vertice %d: ", i);
        for (int j = 0; j < N; ++j)
            printf("disco%d->pino%d ", j, grafo[i].conf[j]);
        printf("\n");
    }
    printf("\n");
}

//-----------------------------------------------
void imprimirMatrizAdjacencia() {
    printf("Matriz de Adjacencia %dx%d:\n", MAX, MAX);
    
    printf("    ");
    for (int j = 0; j < MAX; j++) {
        printf("%3d", j);
    }
    printf("\n");
    printf("    ");
    for (int j = 0; j < MAX; j++) {
        printf("---");
    }
    printf("\n");
    
    for (int i = 0; i < MAX; i++) {
        printf("%2d |", i);
        for (int j = 0; j < MAX; j++) {
            printf("%3d", adj[i][j]);
        }
        printf("\n");
    }
    
    // Estatísticas
    int total_arestas = 0;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (adj[i][j] == 1) total_arestas++;
        }
    }
    printf("\nTotal de arestas: %d\n", total_arestas);
}

//-----------------------------------------------
int main() {
    vertice grafo[MAX];

    printf("Teste da matriz de adjacencia para Torre de Hanoi com %d discos\n", N);
    printf("Numero de configuracoes: %d\n\n", MAX);

    configuracao(grafo);
    imprimirConfiguracoes(grafo);
    
    adjacencia(grafo);
    imprimirMatrizAdjacencia();
    
    return 0;
}
