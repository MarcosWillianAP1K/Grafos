#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PINOS 3
#define DISCOS 4
#define ESTADOS 81 // 3^4 = 81 estados

// Estrutura para o grafo
typedef struct {
    int vertices; // Número de vértices (estados)
    int** matriz; // Matriz de adjacência
} Grafo;

// Inicializa o grafo com verificação de erro
Grafo* inicializarGrafo(int vertices) {
    Grafo* grafo = NULL;
    int sucesso = 1;
    
    if (vertices > 0) {
        grafo = (Grafo*) malloc(sizeof(Grafo));
        if (grafo != NULL) {
            grafo->vertices = vertices;
            grafo->matriz = (int**) malloc(vertices * sizeof(int*));
            if (grafo->matriz != NULL) {
                for (int i = 0; i < vertices && sucesso; i++) {
                    grafo->matriz[i] = (int*) calloc(vertices, sizeof(int));
                    if (grafo->matriz[i] == NULL) {
                        sucesso = 0;
                    }
                }
            } else {
                sucesso = 0;
            }
        } else {
            sucesso = 0;
        }
    } else {
        sucesso = 0;
    }
    
    if (!sucesso && grafo != NULL) {
        if (grafo->matriz != NULL) {
            for (int i = 0; i < vertices; i++) {
                free(grafo->matriz[i]);
            }
            free(grafo->matriz);
        }
        free(grafo);
        grafo = NULL;
    }
    
    return grafo;
}

// Libera a memória do grafo
void liberarGrafo(Grafo* grafo) {
    if (grafo != NULL) {
        if (grafo->matriz != NULL) {
            for (int i = 0; i < grafo->vertices; i++) {
                if (grafo->matriz[i] != NULL) {
                    free(grafo->matriz[i]);
                }
            }
            free(grafo->matriz);
        }
        free(grafo);
    }
}

// Converte estado para índice único
int estadoParaIndice(int* estado) {
    int indice = 0;
    for (int i = 0; i < DISCOS; i++) {
        indice = indice * PINOS + estado[i];
    }
    return indice;
}

// Converte índice para estado
void indiceParaEstado(int indice, int* estado) {
    for (int i = DISCOS - 1; i >= 0; i--) {
        estado[i] = indice % PINOS;
        indice /= PINOS;
    }
}

// Formata estado como (p1,p2,p3,p4)
void formatarEstado(int* estado, char* buffer) {
    sprintf(buffer, "(%d,%d,%d,%d)", estado[0], estado[1], estado[2], estado[3]);
}

// Verifica se um movimento é legal
int movimentoLegal(int* estadoOrigem, int pinoOrigem, int pinoDestino) {
    int isLegal = 0;
    
    int discoOrigem = -1;
    for (int i = 0; i < DISCOS; i++) {
        if (estadoOrigem[i] == pinoOrigem && (discoOrigem == -1 || i < discoOrigem)) {
            discoOrigem = i;
        }
    }
    
    if (discoOrigem != -1) {
        int discoDestino = -1;
        for (int i = 0; i < DISCOS; i++) {
            if (estadoOrigem[i] == pinoDestino && (discoDestino == -1 || i < discoDestino)) {
                discoDestino = i;
            }
        }
        
        if (discoDestino == -1 || discoOrigem < discoDestino) {
            isLegal = 1;
        }
    }
    
    return isLegal;
}

// Constrói o grafo da Torre de Hanói
void construirGrafoHanoi(Grafo* grafo) {
    if (grafo != NULL) {
        int estadoOrigem[DISCOS], estadoDestino[DISCOS];
        
        for (int i = 0; i < grafo->vertices; i++) {
            indiceParaEstado(i, estadoOrigem);
            
            for (int pinoOrigem = 0; pinoOrigem < PINOS; pinoOrigem++) {
                for (int pinoDestino = 0; pinoDestino < PINOS; pinoDestino++) {
                    if (pinoOrigem != pinoDestino && movimentoLegal(estadoOrigem, pinoOrigem, pinoDestino)) {
                        memcpy(estadoDestino, estadoOrigem, DISCOS * sizeof(int));
                        int discoMovido = 0;
                        for (int d = 0; d < DISCOS && !discoMovido; d++) {
                            if (estadoOrigem[d] == pinoOrigem) {
                                estadoDestino[d] = pinoDestino;
                                discoMovido = 1;
                            }
                        }
                        int indiceDestino = estadoParaIndice(estadoDestino);
                        grafo->matriz[i][indiceDestino] = 1;
                    }
                }
            }
        }
    }
}

// Salva a matriz em arquivo
void salvarMatriz(Grafo* grafo, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    int sucesso = (arquivo != NULL);
    
    if (sucesso && grafo != NULL) {
        fprintf(arquivo, "Matriz de Adjacência (%d x %d):\n", grafo->vertices, grafo->vertices);
        fprintf(arquivo, "Estado\\Estado ");
        char buffer[16];
        for (int j = 0; j < grafo->vertices; j++) {
            int estado[DISCOS];
            indiceParaEstado(j, estado);
            formatarEstado(estado, buffer);
            fprintf(arquivo, "%-10s ", buffer);
        }
        fprintf(arquivo, "\n");

        for (int i = 0; i < grafo->vertices; i++) {
            int estado[DISCOS];
            indiceParaEstado(i, estado);
            formatarEstado(estado, buffer);
            fprintf(arquivo, "%-13s ", buffer);
            for (int j = 0; j < grafo->vertices; j++) {
                fprintf(arquivo, "%-10d ", grafo->matriz[i][j]);
            }
            fprintf(arquivo, "\n");
        }
        fclose(arquivo);
        printf("Matriz salva em %s\n", nomeArquivo);
    } else {
        printf("Erro ao abrir o arquivo %s ou grafo inválido!\n", nomeArquivo);
    }
}

// Exibe resumo das conexões
void exibirResumoConexoes(Grafo* grafo) {
    if (grafo != NULL) {
        printf("\nResumo das Conexões (arestas existentes):\n");
        char bufferOrigem[16], bufferDestino[16];
        for (int i = 0; i < grafo->vertices; i++) {
            int estadoOrigem[DISCOS];
            indiceParaEstado(i, estadoOrigem);
            formatarEstado(estadoOrigem, bufferOrigem);
            int count = 0;
            for (int j = 0; j < grafo->vertices; j++) {
                if (grafo->matriz[i][j] == 1) {
                    count++;
                }
            }
            printf("Estado %s: ", bufferOrigem);
            for (int j = 0; j < grafo->vertices; j++) {
                if (grafo->matriz[i][j] == 1) {
                    int estadoDestino[DISCOS];
                    indiceParaEstado(j, estadoDestino);
                    formatarEstado(estadoDestino, bufferDestino);
                    printf("%s ", bufferDestino);
                }
            }
            printf("(%d conexões)\n", count);
        }
    } else {
        printf("Grafo inválido!\n");
    }
}

// Exibe configuração de um estado
void exibirConfiguracaoEstado(int indice) {
    int valido = (indice >= 0 && indice < ESTADOS);
    
    if (valido) {
        int estado[DISCOS];
        indiceParaEstado(indice, estado);
        char buffer[16];
        formatarEstado(estado, buffer);
        
        int pinos[PINOS][DISCOS];
        int tamanhos[PINOS] = {0, 0, 0};
        
        for (int p = 0; p < PINOS; p++) {
            tamanhos[p] = 0;
        }
        
        for (int d = 0; d < DISCOS; d++) {
            int pino = estado[d];
            pinos[pino][tamanhos[pino]] = d + 1;
            tamanhos[pino]++;
        }
        
        printf("\nConfiguração do estado %s:\n", buffer);
        for (int p = 0; p < PINOS; p++) {
            printf("Pino %d: ", p);
            for (int i = tamanhos[p] - 1; i >= 0; i--) {
                printf("%d ", pinos[p][i]);
            }
            if (tamanhos[p] == 0) {
                printf("(vazio)");
            }
            printf("\n");
        }
    } else {
        printf("Índice inválido! Deve estar entre 0 e %d.\n", ESTADOS - 1);
    }
}

// Exibe menu e lê opção
int exibirMenu() {
    int opcao = 0;
    printf("\n=== Menu Torre de Hanói ===\n");
    printf("1. Exibir resumo das conexões\n");
    printf("2. Salvar matriz de adjacência em arquivo\n");
    printf("3. Exibir configuração de um estado\n");
    printf("4. Sair\n");
    printf("Escolha uma opção (1-4): ");
    int resultado = scanf("%d", &opcao);
    while (getchar() != '\n');
    if (resultado != 1 || opcao < 1 || opcao > 4) {
        opcao = 0;
    }
    return opcao;
}

int main() {
    Grafo* grafo = inicializarGrafo(ESTADOS);
    int sucesso = (grafo != NULL);
    
    if (sucesso) {
        construirGrafoHanoi(grafo);
        int rodando = 1;
        while (rodando) {
            int opcao = exibirMenu();
            
            if (opcao == 1) {
                exibirResumoConexoes(grafo);
            } else if (opcao == 2) {
                salvarMatriz(grafo, "matriz_hanoi.txt");
            } else if (opcao == 3) {
                int indice = 0;
                printf("Digite o índice do estado (0 a %d): ", ESTADOS - 1);
                int resultado = scanf("%d", &indice);
                while (getchar() != '\n');
                if (resultado == 1) {
                    exibirConfiguracaoEstado(indice);
                } else {
                    printf("Entrada inválida! Digite um número entre 0 e %d.\n", ESTADOS - 1);
                }
            } else if (opcao == 4) {
                rodando = 0;
            } else {
                printf("Opção inválida! Escolha entre 1 e 4.\n");
            }
        }
        liberarGrafo(grafo);
    } else {
        printf("Erro ao inicializar o grafo!\n");
    }
    
    return 0;
}