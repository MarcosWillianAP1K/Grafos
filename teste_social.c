#include <stdio.h>
#include <stdlib.h>

// Lista encadeada
typedef struct ARESTA
{
    int vertice_destino; // Identificador do vértice de destino
    int peso;            // Peso da aresta, se for ponderada
    struct ARESTA *proximo;
} ARESTA;

typedef struct VERTICE
{
    int id;   // Identificador do vértice
    int grau; // Grau do vértice
    int peso; // Peso do vértice, se for ponderado
} VERTICE;

typedef struct VETOR_ADJACENCIA
{
    VERTICE vertice;       // Vértice ao qual a aresta de adjacência pertence
    ARESTA *lista_aresta; // lista de arestas
} VETOR_ADJACENCIA;

typedef struct GRAFO
{
    short int eh_ponderado;             // 1 se o grafo é ponderado, 0 se não
    short int eh_digrafo;               // 1 se o grafo é um dígrafo, 0 se não
    int grau_max;                       // Grau máximo do grafo
    VETOR_ADJACENCIA *vetor_adjacencia; // Vetor de listas de adjacência
} GRAFO;

void verificar_alocacao(void *ptr, const char *mensagem)
{
    if (!ptr)
    {
        fprintf(stderr, "Erro de alocação: %s\n", mensagem);
        exit(EXIT_FAILURE);
    }
}

// Função para criar um grafo com grau máximo, ponderado e direcionado
GRAFO *criar_grafo(int grau_max, short int eh_ponderado, short int eh_digrafo)
{
    GRAFO *grafo = (GRAFO *)malloc(sizeof(GRAFO));

    verificar_alocacao(grafo, "falha ao alocar memória para o grafo");

    grafo->eh_ponderado = eh_ponderado;
    grafo->eh_digrafo = eh_digrafo;
    grafo->grau_max = grau_max;
    grafo->vetor_adjacencia = (VETOR_ADJACENCIA *)malloc(grau_max * sizeof(VETOR_ADJACENCIA));

    verificar_alocacao(grafo->vetor_adjacencia, "falha ao alocar memória para o vetor de adjacência");

    // Inicializa o vetor de adjacência, o grau maximo deve ser somado +1 pois e a quantidade de vertices possiveis de linkar mais o vertice 0
    for (int i = 0; i < grau_max + 1; i++)
    {
        grafo->vetor_adjacencia[i].vertice.id = i;
        grafo->vetor_adjacencia[i].vertice.grau = 0;
        grafo->vetor_adjacencia[i].vertice.peso = 0;
        grafo->vetor_adjacencia[i].lista_aresta = NULL;
    }

    return grafo;
}

// Função para liberar a memória alocada para o grafo
void liberar_grafo(GRAFO **grafo)
{
    if (grafo != NULL && *grafo != NULL)
    {
        for (int i = 0; i < (*grafo)->grau_max; i++)
        {
            ARESTA *aresta_atual = (*grafo)->vetor_adjacencia[i].lista_aresta;
            while (aresta_atual)
            {
                ARESTA *temp = aresta_atual;
                aresta_atual = aresta_atual->proximo;
                free(temp);
            }
        }
        free((*grafo)->vetor_adjacencia);
        free(*grafo);
        *grafo = NULL;
    }
}


int main()
{
    int grau_max = 5;
    short int eh_ponderado = 1; // Grafo ponderado
    short int eh_digrafo = 0;   // Grafo não direcionado

    GRAFO *grafo = criar_grafo(grau_max, eh_ponderado, eh_digrafo);

    printf("Grafo criado com sucesso!\n");
    printf("Grau máximo: %d\n", grafo->grau_max);
    printf("É ponderado: %d\n", grafo->eh_ponderado);
    printf("É dígrafo: %d\n", grafo->eh_digrafo);

    // Libera a memória alocada para o grafo
    liberar_grafo(&grafo);

    return 0;
}