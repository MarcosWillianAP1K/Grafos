#include <stdio.h>
#include <stdlib.h>

#define NUMERO_DE_DISCOS 2
#define NUMERO_DE_PINOS 3
#define INFINITO 999999999 // Um valor grande o suficiente para representar o infinito

// Converte uma configuração de discos (vetor) para um valor inteiro único
int converter_configuracao_para_inteiro(int configuracao[NUMERO_DE_DISCOS]) {
    int valor = 0;
    for (int i = 0; i < NUMERO_DE_DISCOS; i++) {
        valor = valor * NUMERO_DE_PINOS + configuracao[i];
    }
    return valor;
}

// Converte um valor inteiro único para uma configuração de discos (vetor)
void converter_inteiro_para_configuracao(int valor, int configuracao[NUMERO_DE_DISCOS]) {
    for (int i = NUMERO_DE_DISCOS - 1; i >= 0; i--) {
        configuracao[i] = valor % NUMERO_DE_PINOS;
        valor /= NUMERO_DE_PINOS;
    }
}

// Verifica se um disco é o disco superior em um pino
int eh_disco_superior(int configuracao[NUMERO_DE_DISCOS], int disco_idx, int pino) {
    int resultado = 1; // Assume que é o disco superior
    int i = 0;
    while (i < disco_idx) {
        if (configuracao[i] == pino) {
            resultado = 0; // Há um disco menor no mesmo pino
        }
        i++;
    }
    return resultado;
}

// Verifica se o pino de destino pode receber o disco
int pode_receber_disco(int configuracao[NUMERO_DE_DISCOS], int disco_idx, int pino_destino) {
    int resultado = 1; // Assume que pode receber
    int i = 0;
    while (i < NUMERO_DE_DISCOS) {
        if (configuracao[i] == pino_destino && i < disco_idx) {
            resultado = 0; // Há um disco menor no pino de destino
        }
        i++;
    }
    return resultado;
}

// Gera a matriz de adjacência do grafo de estados da Torre de Hanoi
void gerar_grafo(int numero_de_estados, int** matriz_adjacencia) {
    for (int i = 0; i < numero_de_estados; i++) {
        for (int j = 0; j < numero_de_estados; j++) {
            matriz_adjacencia[i][j] = 0; // Inicializa sem conexões
        }
    }

    int configuracao_atual[NUMERO_DE_DISCOS];
    int proxima_configuracao[NUMERO_DE_DISCOS];

    for (int i = 0; i < numero_de_estados; i++) {
        converter_inteiro_para_configuracao(i, configuracao_atual);

        for (int disco_idx = 0; disco_idx < NUMERO_DE_DISCOS; disco_idx++) {
            int pino_origem = configuracao_atual[disco_idx];

            if (eh_disco_superior(configuracao_atual, disco_idx, pino_origem)) {
                for (int pino_destino = 0; pino_destino < NUMERO_DE_PINOS; pino_destino++) {
                    if (pino_origem != pino_destino) {
                        if (pode_receber_disco(configuracao_atual, disco_idx, pino_destino)) {
                            // Copia a configuração atual para simular o movimento
                            for (int k = 0; k < NUMERO_DE_DISCOS; k++) {
                                proxima_configuracao[k] = configuracao_atual[k];
                            }
                            proxima_configuracao[disco_idx] = pino_destino;

                            int proximo_estado = converter_configuracao_para_inteiro(proxima_configuracao);
                            matriz_adjacencia[i][proximo_estado] = 1; // Adiciona aresta com peso 1
                        }
                    }
                }
            }
        }
    }
}

// Encontra o nó com a menor distância não visitada
int encontrar_no_menor_distancia(int numero_de_estados, int distancias[], int visitados[]) {
    int menor_distancia = INFINITO;
    int indice_menor = -1;

    for (int v = 0; v < numero_de_estados; v++) {
        if (visitados[v] == 0 && distancias[v] < menor_distancia) {
            menor_distancia = distancias[v];
            indice_menor = v;
        }
    }
    return indice_menor;
}

// Implementação do algoritmo de Dijkstra para encontrar o menor caminho
int encontrar_menor_caminho_dijkstra(int numero_de_estados, int** matriz_adjacencia, int no_inicial, int no_final) {
    int distancias[numero_de_estados];
    int visitados[numero_de_estados];

    for (int i = 0; i < numero_de_estados; i++) {
        distancias[i] = INFINITO;
        visitados[i] = 0;
    }

    distancias[no_inicial] = 0;

    int contador = 0;
    int u = 0;
    while (contador < numero_de_estados - 1 && u != -1) {
        u = encontrar_no_menor_distancia(numero_de_estados, distancias, visitados);

        if (u != -1) {
            visitados[u] = 1;

            for (int v = 0; v < numero_de_estados; v++) {
                if (!visitados[v] && matriz_adjacencia[u][v] && distancias[u] != INFINITO) {
                    if (distancias[u] + matriz_adjacencia[u][v] < distancias[v]) {
                        distancias[v] = distancias[u] + matriz_adjacencia[u][v];
                    }
                }
            }
        }
        contador++;
    }

    return distancias[no_final];
}

// Função para imprimir a matriz de adjacência com numeração nas linhas e colunas
void imprimir_matriz_adjacencia(int numero_de_estados, int** matriz_adjacencia) {
    printf("\nMatriz de Adjacência:\n");

    // Imprime os números das colunas
    printf("   ");
    for (int j = 0; j < numero_de_estados; j++) {
        printf("%2d ", j);
    }
    printf("\n");

    // Imprime uma linha separadora
    printf("   ");
    for (int j = 0; j < numero_de_estados; j++) {
        printf("---");
    }
    printf("\n");

    // Imprime a matriz com os números das linhas
    for (int i = 0; i < numero_de_estados; i++) {
        printf("%2d|", i);
        for (int j = 0; j < numero_de_estados; j++) {
            printf("%2d ", matriz_adjacencia[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int numero_de_estados = 1;
    for (int i = 0; i < NUMERO_DE_DISCOS; i++) {
        numero_de_estados *= NUMERO_DE_PINOS;
    }

    // Alocação dinâmica da matriz de adjacência
    int** matriz_adjacencia = (int**)malloc(numero_de_estados * sizeof(int*));
    if (matriz_adjacencia == NULL) {
        fprintf(stderr, "Erro de alocação de memória para matriz_adjacencia.\n");
        return 1;
    }
    for (int i = 0; i < numero_de_estados; i++) {
        matriz_adjacencia[i] = (int*)calloc(numero_de_estados, sizeof(int));
        if (matriz_adjacencia[i] == NULL) {
            fprintf(stderr, "Erro de alocação de memória para linha %d.\n", i);
            // Libera memória alocada até agora
            for (int j = 0; j < i; j++) {
                free(matriz_adjacencia[j]);
            }
            free(matriz_adjacencia);
            return 1;
        }
    }

    gerar_grafo(numero_de_estados, matriz_adjacencia);
    imprimir_matriz_adjacencia(numero_de_estados, matriz_adjacencia);

    // Define a configuração inicial (todos os discos no pino 0)
    int configuracao_inicial[NUMERO_DE_DISCOS];
    for (int i = 0; i < NUMERO_DE_DISCOS; i++) {
        configuracao_inicial[i] = 0;
    }
    int no_inicial = converter_configuracao_para_inteiro(configuracao_inicial);

    // Define a configuração final (todos os discos no pino 2)
    int configuracao_final[NUMERO_DE_DISCOS];
    for (int i = 0; i < NUMERO_DE_DISCOS; i++) {
        configuracao_final[i] = 2;
    }
    int no_final = converter_configuracao_para_inteiro(configuracao_final);

    printf("Calculando o menor caminho para a Torre de Hanoi com %d discos...\n", NUMERO_DE_DISCOS);
    int menor_caminho = encontrar_menor_caminho_dijkstra(numero_de_estados, matriz_adjacencia, no_inicial, no_final);

    if (menor_caminho == INFINITO) {
        printf("Não foi possível encontrar um caminho do estado inicial ao estado final.\n");
    } else {
        printf("O menor caminho do estado inicial ao estado final é: %d movimentos.\n", menor_caminho);
    }

    // Liberação da memória
    for (int i = 0; i < numero_de_estados; i++) {
        free(matriz_adjacencia[i]);
    }
    free(matriz_adjacencia);

    return 0;
}