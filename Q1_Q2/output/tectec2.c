#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 2       // Número de discos
#define Pinos 3
#define MAX 9 // Pinos ^ N

const int INF = 0x3f3f3f3f;

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

    int invalido = 1;

    for (int i = 0; i < N && invalido; ++i) {
        if (i != disco_alterado && ((v1.conf[i] == v1.conf[disco_alterado] && i < disco_alterado) || (v2.conf[i] == v2.conf[disco_alterado] && i < disco_alterado))) {
            invalido = 0;
        }
    }

    return (diff > 1 || !invalido) ? 0 : 1;
}
//-----------------------------------------------
void adjacencia(vertice *grafo) {
    for (int i = 0; i < MAX; ++i)
        for (int j = 0; j < MAX; ++j)
            adj[i][j] = aresta(grafo[i], grafo[j]) ? 1 : INF;
}
//-----------------------------------------------

void caminhoMinimo(int *p, int s, int f) {
    printf("Caminho minimo entre configurações %d e %d:\n", s, f);

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
    for (int i = 0; i < MAX - 1; ++i) {
        int relaxado = 0;
        for (int j = 0; j < MAX; ++j) {
            if (dp[j] != INF) {
                for (int k = 0; k < MAX; k++) {
                    if (adj[j][k] != INF && dp[j] + adj[j][k] < dp[k]) {
                        dp[k] = dp[j] + adj[j][k];
                        p[k] = j;
                        relaxado = 1;
                    }
                }
            }
        }
        if (!relaxado) break;
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
int main() {
    vertice grafo[MAX];

    configuracao(grafo);
    imprimir(grafo);
    adjacencia(grafo);

    int inicio, fim;

    printf("Digite a configuracao inicial:\n"); scanf("%d", &inicio);
    printf("Digite a configuracao final:\n"); scanf("%d", &fim); 

    fordMooreBellman(inicio, fim);

    return 0;
}