#include <stdio.h>
#include <stdlib.h>

#define N 81
#define INF 999999

int adj[N][N];

int config_para_indice(int config[4]) {
    int indice = (config[0]-1)*27 + (config[1]-1)*9 + (config[2]-1)*3 + (config[3]-1);
    return indice;
}

void indice_para_config(int indice, int config[4]) {
    config[3] = (indice % 3) + 1;
    indice /= 3;
    config[2] = (indice % 3) + 1;
    indice /= 3;
    config[1] = (indice % 3) + 1;
    config[0] = indice + 1;
}

int config_valida(int config[4], int disco, int origem, int destino) {
    int valida = 1;
    if (disco >= 0) {
        if (config[disco] != origem) {
            valida = 0;
        } else {
            int d = 0;
            while (d < 4) {
                if (d != disco && config[d] == origem && d < disco) {
                    valida = 0;
                }
                d++;
            }
            if (valida) {
                int topo_destino = -1;
                int i = 0;
                while (i < 4) {
                    if (config[i] == destino && i < disco) {
                        topo_destino = i;
                    }
                    i++;
                }
                if (topo_destino != -1 && topo_destino < disco) {
                    valida = 0;
                }
            }
        }
    } else {
        int pino = 1;
        while (pino <= 3) {
            int ultimo_tamanho = 5;
            int d = 3;
            while (d >= 0) {
                if (config[d] == pino) {
                    if (d + 1 > ultimo_tamanho) {
                        valida = 0;
                    }
                    ultimo_tamanho = d + 1;
                }
                d--;
            }
            pino++;
        }
    }
    return valida;
}

void gerar_grafo() {
    int i = 0;
    while (i < N) {
        int j = 0;
        while (j < N) {
            adj[i][j] = 0;
            j++;
        }
        int config[4];
        indice_para_config(i, config);
        if (config_valida(config, -1, 0, 0)) {
            int disco = 0;
            while (disco < 4) {
                int origem = config[disco];
                int destino = 1;
                while (destino <= 3) {
                    if (destino != origem) {
                        int nova_config[4];
                        int k = 0;
                        while (k < 4) {
                            nova_config[k] = config[k];
                            k++;
                        }
                        nova_config[disco] = destino;
                        if (config_valida(nova_config, disco, origem, destino)) {
                            j = config_para_indice(nova_config);
                            adj[i][j] = 1;
                        }
                    }
                    destino++;
                }
                disco++;
            }
        }
        i++;
    }
}

void exibir_matriz() {
    int i = 0;
    while (i < N) {
        int j = 0;
        while (j < N) {
            printf("%d", adj[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

void dijkstra(int inicio, int fim) {
    int dist[N];
    int visited[N];
    int prev[N];
    int i = 0;
    while (i < N) {
        dist[i] = INF;
        visited[i] = 0;
        prev[i] = -1;
        i++;
    }
    dist[inicio] = 0;
    int count = 0;
    while (count < N) {
        int u = 0;
        i = 1;
        while (i < N) {
            if (!visited[i] && dist[i] < dist[u]) {
                u = i;
            }
            i++;
        }
        if (visited[u] || dist[u] == INF) {
            count = N;
        } else {
            visited[u] = 1;
            i = 0;
            while (i < N) {
                if (!visited[i] && adj[u][i] && dist[u] + 1 < dist[i]) {
                    dist[i] = dist[u] + 1;
                    prev[i] = u;
                }
                i++;
            }
            count++;
        }
    }
    if (dist[fim] == INF) {
        printf("Caminho nao existe\n");
    } else {
        int caminho[N];
        int tam = 0;
        int v = fim;
        while (v != -1) {
            caminho[tam] = v;
            tam++;
            v = prev[v];
        }
        printf("Caminho mais curto (%d movimentos):\n", dist[fim]);
        i = tam - 1;
        while (i >= 0) {
            int config[4];
            indice_para_config(caminho[i], config);
            printf("Configuracao: [%d,%d,%d,%d]\n", config[0], config[1], config[2], config[3]);
            i--;
        }
    }
}

void exibir_menu() {
    printf("\nMenu\n1. Matriz\n2. Dijkstra\n3. Sair\nOpcao: ");
}

int processar_opcao(int opcao) {
    int continuar = 1;
    int config_inicio[4] = {1, 1, 1, 1};
    int config_fim[4] = {3, 3, 3, 3};
    int inicio = config_para_indice(config_inicio);
    int fim = config_para_indice(config_fim);
    
    if (opcao == 1) {
        exibir_matriz();
    } else if (opcao == 2) {
        dijkstra(inicio, fim);
    } else if (opcao == 3) {
        continuar = 0;
    } else {
        printf("Invalido\n");
    }
    return continuar;
}

int main() {
    gerar_grafo();
    int continuar = 1;
    while (continuar) {
        exibir_menu();
        int opcao;
        scanf("%d", &opcao);
        continuar = processar_opcao(opcao);
    }
    return 0;
}