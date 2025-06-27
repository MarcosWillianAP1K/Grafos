#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_DISCOS 2  // Número de discos
#define NUM_PINOS 3
#define MAX 9 // NUM_PINOS ^ NUM_DISCOS

const int INF = 99999999; 

typedef struct {
    int conf[NUM_DISCOS]; // Configuração dos discos nos pinos
} NoGrafo; 

int adj[MAX][MAX]; // Matriz de adjacência

//-----------------------------------------------
// Gera todas as configurações possíveis dos discos
void gerarConfiguracoes(NoGrafo *grafo) {
    for (int i = 0; i < MAX; ++i) {
        int numero_base = i;
        for (int j = 0; j < NUM_DISCOS; ++j) {
            grafo[i].conf[j] = numero_base % NUM_PINOS + 1;
            numero_base /= NUM_PINOS;
        }
    }
}
//-----------------------------------------------
// Verifica se há uma aresta válida entre duas configurações
int ehAresta(NoGrafo config1, NoGrafo config2) {
    int diferenca_discos = 0;
    int disco_alterado = -1;
    int valida = 1; // Assume que a aresta é válida por padrão

    for (int i = 0; i < NUM_DISCOS; ++i) {
        if (config1.conf[i] != config2.conf[i]) {
            diferenca_discos++;
            disco_alterado = i;
        }
    }

    valida = (diferenca_discos == 1);

    if (valida) {
        for (int i = 0; i < NUM_DISCOS; ++i) {
            if (i != disco_alterado && i < disco_alterado) { 
                if (config1.conf[i] == config1.conf[disco_alterado] || config2.conf[i] == config2.conf[disco_alterado]) {
                    valida = 0; 
                }
            }
        }
    }
    
    return valida; 
}
//-----------------------------------------------
// Preenche a matriz de adjacência do grafo
void gerarAdjacencias(NoGrafo *grafo) {
    for (int i = 0; i < MAX; ++i) {
        for (int j = 0; j < MAX; ++j) {
            adj[i][j] = ehAresta(grafo[i], grafo[j]);
        }
    }
}
//-----------------------------------------------

// Imprime o caminho mínimo encontrado
void imprimirCaminho(int *p, int s, int f) {
    printf("Caminho minimo entre configuracoes %d e %d:\n", s, f);

    int caminho[f + 1], temp = p[f], pos = 0;

    while(temp != -1) temp = p[caminho[pos++] = temp];

    for (int i = pos - 1; ~i; i--) printf("%d ", caminho[i]);

    printf("%d\n", f);
}
//-----------------------------------------------
// Implementação do algoritmo de Ford-Moore-Bellman
void fordMooreBellman(int s, int fim) {
    int dp[MAX], p[MAX];

    memset(p, -1, sizeof(p));

    for (int i = 0; i < MAX; i++) dp[i] = INF;
    dp[s] = 0;
    for (int i = 0; i < MAX - 1; ++i) {
        for (int j = 0; j < MAX; ++j) {
            if (dp[j] != INF) {
                for (int k = 0; k < MAX; k++) {
                    if (adj[j][k] == 1 && dp[j] + adj[j][k] < dp[k]) {
                        dp[k] = dp[j] + adj[j][k];
                        p[k] = j;
                    }
                }
            }
        }
    }
    printf("Distancia minima (Ford-Moore-Bellman): %d\n", dp[fim]);
    imprimirCaminho(p, s, fim);
}

//-----------------------------------------------
// Implementação do algoritmo de Dijkstra
void dijkstra(int s, int fim) {
    int dp[MAX], p[MAX];
    int visitado[MAX] = {0};

    memset(p, -1, sizeof(p));
    for (int i = 0; i < MAX; i++) dp[i] = INF;
    dp[s] = 0;

    for (int i = 0; i < MAX; ++i) {
        int u = -1, min_dist = INF;
        for (int j = 0; j < MAX; ++j) {
            if (!visitado[j] && dp[j] < min_dist) {
                min_dist = dp[j];
                u = j;
            }
        }

        visitado[u] = 1;

        for (int v = 0; v < MAX; ++v) {
            if (adj[u][v] == 1 && dp[u] + adj[u][v] < dp[v]) {
                dp[v] = dp[u] + adj[u][v];
                p[v] = u;
            }
        }
    }
    printf("Distancia minima (Dijkstra): %d\n", dp[fim]);
    imprimirCaminho(p, s, fim);
}

//-----------------------------------------------
// Imprime todas as configurações dos vértices do grafo
void imprimirGrafo(NoGrafo *grafo) {
    for (int i = 0; i < MAX; ++i) {
        printf("Configuracao %d: ", i);
        for (int j = 0; j < NUM_DISCOS; ++j) {
            printf("%d ", grafo[i].conf[j]);
        }
        printf("\n");
    }
}
//-----------------------------------------------

// Imprime a matriz de adjacência
void imprimirMatrizAdjacencia() {
    printf("\nMatriz de Adjacencia:\n");
    printf("   ");
    for (int i = 0; i < MAX; i++) {
        printf("%2d ", i);
    }
    printf("\n");
    for (int i = 0; i < MAX; i++) {
        printf("%2d ", i);
        for (int j = 0; j < MAX; j++) {
            printf("%2d ", adj[i][j]);
        }
        printf("\n");
    }
}
//-----------------------------------------------
int main() {
    NoGrafo grafo[MAX];

    gerarConfiguracoes(grafo);
    gerarAdjacencias(grafo);

    int opcao;
    int origem;
    int destino;

    do {
        printf("\n\n==== MENU ====\n");
        printf("1. Imprimir configuracoes do grafo\n");
        printf("2. Imprimir matriz de adjacencia\n");
        printf("3. Encontrar caminho minimo com Ford-Moore-Bellman\n");
        printf("4. Encontrar caminho minimo com Dijkstra\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                imprimirGrafo(grafo);
                break;
            case 2:
                imprimirMatrizAdjacencia();
                break;
            case 3:
                printf("Digite a configuracao de origem: ");
                scanf("%d", &origem);
                printf("Digite a configuracao de destino: ");
                scanf("%d", &destino);
                if (origem >= 0 && origem < MAX && destino >= 0 && destino < MAX) {
                    fordMooreBellman(origem, destino);
                } else {
                    printf("Configuracoes invalidas!\n");
                }
                break;
            case 4:
                printf("Digite a configuracao de origem: ");
                scanf("%d", &origem);
                printf("Digite a configuracao de destino: ");
                scanf("%d", &destino);
                if (origem >= 0 && origem < MAX && destino >= 0 && destino < MAX) {
                    dijkstra(origem, destino);
                } else {
                    printf("Configuracoes invalidas!\n");
                }
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 5);
    
    return 0;
}