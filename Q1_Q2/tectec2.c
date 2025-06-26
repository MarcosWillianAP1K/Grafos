#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 2       // Número de discos
#define Pinos 3
#define MAX 9 // Pinos ^ N

#define INF 9999999

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

    int valido = (diff == 1); // Válido apenas se exatamente um disco mudou

    if (valido) {
        for (int i = 0; i < N && valido; ++i) {
            if (i != disco_alterado && 
                ((v1.conf[i] == v1.conf[disco_alterado] && i < disco_alterado) || (v2.conf[i] == v2.conf[disco_alterado] && i < disco_alterado))) {
                valido = 0;
            }
        }
    }

    return valido;
}
//-----------------------------------------------
void adjacencia(vertice *grafo) {
    for (int i = 0; i < MAX; ++i)
        for (int j = 0; j < MAX; ++j)
            adj[i][j] = aresta(grafo[i], grafo[j]) ? 1 : 0;// mudou aqui
}
//-----------------------------------------------

void caminhoMinimo(int *p, int s, int f) {
    printf("Caminho minimo entre configurações %d e %d:\n", s, f);

    int caminho[f + 1], temp = p[f], pos = 0;

    while (temp != -1) {
        temp = p[caminho[pos++] = temp];
    }

    for (int i = pos - 1; i >= 0; i--) {
        printf("%d ", caminho[i]);
    }

    printf("%d\n", f);
}
//-----------------------------------------------
void fordMooreBellman(int s, int fim) {
    int dp[MAX], p[MAX];
    
    memset(p, -1, sizeof(p));
    
    for (int i = 0; i < MAX; i++) dp[i] = INF;
    dp[s] = 0;
    for (int i = 0; i < MAX - 1; ++i) {
        for (int j = 0; j < MAX; ++j) {
            if (dp[j] != INF) {
                for (int k = 0; k < MAX; k++) {
                    if (adj[j][k] != INF && dp[j] + adj[j][k] < dp[k]) {
                        dp[k] = dp[j] + adj[j][k];
                        p[k] = j;
                    }
                }
            }
        }
    }
    caminhoMinimo(p, s, fim);
}

//-----------------------------------------------

void dijkstra(int s, int fim) {
    int dp[MAX], p[MAX];
    int visitado[MAX] = {0};
    
    memset(p, -1, sizeof(p));
    for (int i = 0; i < MAX; i++) dp[i] = INF;
    dp[s] = 0;

    for (int i = 0; i < MAX; ++i) {
        int u = -1;
        for (int j = 0; j < MAX; ++j) {
            if (!visitado[j] && (u == -1 || dp[j] < dp[u])) {
                u = j;
            }
        }

        if (dp[u] != INF) {
            visitado[u] = 1;

            for (int v = 0; v < MAX; ++v) {
                if (adj[u][v] != INF && dp[u] + adj[u][v] < dp[v]) {
                    dp[v] = dp[u] + adj[u][v];
                    p[v] = u;
                }
            }
        }
    }

    caminhoMinimo(p, s, fim);
}
//-----------------------------------------------
void imprimir(vertice *grafo) {
    for (int i = 0; i < MAX; ++i) {
        printf("vertice %d: ", i);
        for (int j = 0; j < N; ++j)
            printf("%d ", grafo[i].conf[j]);
        printf("\n");
    }
}
//-----------------------------------------------

void imprimirAdjacencia() {
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
    vertice grafo[MAX];

    configuracao(grafo);
    imprimir(grafo);
    adjacencia(grafo);

    imprimirAdjacencia();

    int inicio, fim;

    int opcao;
    
    do {
        printf("\n\nMenu:\n");
        printf("1 - Exibir configuracoes dos vertices\n");
        printf("2 - Exibir matriz de adjacencia\n");
        printf("3 - Encontrar caminho minimo (Ford-Moore-Bellman)\n");
        printf("4 - Encontrar caminho minimo (Dijkstra)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                imprimir(grafo);
                break;
            case 2:
                imprimirAdjacencia();
                break;
            case 3:{
                clock_t inicio_tempo, fim_tempo;
                double tempo_execucao;

                printf("Digite o vertice inicial: ");
                scanf("%d", &inicio);
                printf("Digite o vertice final: ");
                scanf("%d", &fim);

                inicio_tempo = clock();
                fordMooreBellman(inicio, fim);
                fim_tempo = clock();
                tempo_execucao = (double)(fim_tempo - inicio_tempo) / CLOCKS_PER_SEC;
                printf("Tempo de execucao: %f segundos\n", tempo_execucao);
            }
            case 4:{
                printf("Digite o vertice inicial: ");
                scanf("%d", &inicio);
                printf("Digite o vertice final: ");
                scanf("%d", &fim);
                dijkstra(inicio, fim);
                break;
            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
    return 0;
}