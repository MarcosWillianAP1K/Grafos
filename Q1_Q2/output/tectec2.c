#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 2       // Número de discos
#define Pinos 3
#define MAX 9 // Pinos ^ N

const int INF = 99999999;

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

    for (int i = 0; i < N && valido; ++i) {
        if (i != disco_alterado && i < disco_alterado && 
            (v1.conf[i] == v1.conf[disco_alterado] || v2.conf[i] == v2.conf[disco_alterado])) {
            valido = 0;
        }
    }

    return valido;
}
//-----------------------------------------------
void adjacencia(vertice *grafo) {
    for (int i = 0; i < MAX; ++i)
        for (int j = 0; j < MAX; ++j)
            adj[i][j] = aresta(grafo[i], grafo[j]) ? 1 : 0;
}
//-----------------------------------------------

void caminhoMinimo(int *p, int s, int f) {
    printf("Caminho minimo entre configuracoes %d e %d:\n", s, f);

    int caminho[MAX], temp = p[f], pos = 0;

    while (temp != -1) {
        caminho[pos++] = temp;
        temp = p[temp];
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
    
    int convergiu = 0;
    for (int i = 0; i < MAX - 1 && !convergiu; ++i) {
        int relaxado = 0;
        for (int j = 0; j < MAX; ++j) {
            if (dp[j] != INF) {
                for (int k = 0; k < MAX; k++) {
                    if (adj[j][k] == 1 && dp[j] + 1 < dp[k]) {
                        dp[k] = dp[j] + 1;
                        p[k] = j;
                        relaxado = 1;
                    }
                }
            }
        }
        if (!relaxado) {
            convergiu = 1;
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
    printf("Matriz de adjacencia:\n");
    for (int i = 0; i < MAX; ++i) {
        for (int j = 0; j < MAX; ++j) {
            printf("%d ", adj[i][j]);
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

    int opcao, origem, destino;

    do {
        printf("\n\n==== MENU ====\n");
        printf("1. Imprimir vertices do grafo\n");
        printf("2. Imprimir matriz de adjacencia\n");
        printf("3. Encontrar caminho minimo com Ford-Moore-Bellman\n");
        printf("4. Encontrar caminho minimo com Dijkstra\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                imprimir(grafo);
                break;
            case 2:
                imprimirAdjacencia();
                break;
            case 3:
                printf("Digite o vertice de origem: ");
                scanf("%d", &origem);
                printf("Digite o vertice de destino: ");
                scanf("%d", &destino);
                if (origem >= 0 && origem < MAX && destino >= 0 && destino < MAX)
                    fordMooreBellman(origem, destino);
                else
                    printf("Vertices invalidos!\n");
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