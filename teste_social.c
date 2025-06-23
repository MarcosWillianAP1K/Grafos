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
    VERTICE vertice;      // Vértice ao qual a aresta de adjacência pertence
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
        fprintf(stderr, "Erro de alocacao: %s\n", mensagem);
        exit(EXIT_FAILURE);
    }
}

ARESTA *criar_aresta(int vertice_destino, int peso)
{
    ARESTA *nova_aresta = (ARESTA *)malloc(sizeof(ARESTA));
    verificar_alocacao(nova_aresta, "falha ao alocar memória para a aresta");

    nova_aresta->vertice_destino = vertice_destino;
    nova_aresta->peso = peso;
    nova_aresta->proximo = NULL;

    return nova_aresta;
}

// Função para criar um grafo com grau máximo, ponderado e direcionado
GRAFO *criar_grafo(int grau_max, short int eh_ponderado, short int eh_digrafo)
{
    GRAFO *grafo = (GRAFO *)malloc(sizeof(GRAFO));

    verificar_alocacao(grafo, "falha ao alocar memória para o grafo");

    grafo->eh_ponderado = eh_ponderado;
    grafo->eh_digrafo = eh_digrafo;
    grafo->grau_max = grau_max;
    grafo->vetor_adjacencia = (VETOR_ADJACENCIA *)malloc((grau_max + 1) * sizeof(VETOR_ADJACENCIA));

    verificar_alocacao(grafo->vetor_adjacencia, "falha ao alocar memória para o vetor de adjacência");

    for (int i = 0; i < grau_max + 1; i++)
    {
        grafo->vetor_adjacencia[i].vertice.id = i + 1;
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

short int inserir_aresta(GRAFO *grafo, int vertice_origem, int vertice_destino, int peso)
{
    short int retorno = 0;

    // Verificações necessarias para inserir a aresta
    if (grafo != NULL && vertice_origem > 0 && vertice_origem <= grafo->grau_max && vertice_destino > 0 && vertice_destino <= grafo->grau_max && ((!grafo->eh_ponderado && peso == 1) || (grafo->eh_ponderado)))
    {
        ARESTA *nova_aresta = criar_aresta(vertice_destino, peso);

        nova_aresta->proximo = grafo->vetor_adjacencia[vertice_origem - 1].lista_aresta;
        grafo->vetor_adjacencia[vertice_origem - 1].lista_aresta = nova_aresta;
        grafo->vetor_adjacencia[vertice_origem - 1].vertice.grau++;

        if (!grafo->eh_digrafo)
        {
            // Se o grafo não é direcionado, adiciona a aresta na direção oposta
            ARESTA *nova_aresta_oposta = criar_aresta(vertice_origem, peso);
            nova_aresta_oposta->proximo = grafo->vetor_adjacencia[vertice_destino - 1].lista_aresta;
            grafo->vetor_adjacencia[vertice_destino - 1].lista_aresta = nova_aresta_oposta;
            grafo->vetor_adjacencia[vertice_destino - 1].vertice.grau++;
        }

        retorno = 1; // Indica que a aresta foi inserida com sucesso
    }

    return retorno;
}

short int remover_aresta(GRAFO *grafo, int vertice_origem, int vertice_destino)
{
    short int retorno = 0;

    if (grafo != NULL && vertice_origem > 0 && vertice_origem <= grafo->grau_max && vertice_destino > 0 && vertice_destino <= grafo->grau_max)
    {
        ARESTA *atual = grafo->vetor_adjacencia[vertice_origem - 1].lista_aresta;
        ARESTA *anterior = NULL;

        while (atual != NULL && atual->vertice_destino != vertice_destino)
        {
            anterior = atual;
            atual = atual->proximo;
        }

        if (atual != NULL) // A aresta foi encontrada
        {
            if (anterior == NULL) // A aresta é a primeira da lista
            {
                grafo->vetor_adjacencia[vertice_origem - 1].lista_aresta = atual->proximo;
            }
            else
            {
                anterior->proximo = atual->proximo;
            }

            free(atual);
            grafo->vetor_adjacencia[vertice_origem - 1].vertice.grau--;

            if (!grafo->eh_digrafo)
            {
                // Se o grafo não é direcionado, remove a aresta na direção oposta
                remover_aresta(grafo, vertice_destino, vertice_origem);
            }

            retorno = 1; // Indica que a aresta foi removida com sucesso
        }
    }

    return retorno;
}

void imprimir_matriz_grafo(GRAFO *grafo)
{
    if (grafo != NULL)
    {

        printf("\nMatriz de Adjacencia do Grafo:\n");
        for (int i = 0; i < grafo->grau_max + 1; i++)
        {
            printf("Vertice %d: ", grafo->vetor_adjacencia[i].vertice.id);
            ARESTA *aresta_atual = grafo->vetor_adjacencia[i].lista_aresta;
            while (aresta_atual != NULL)
            {
                if (grafo->eh_ponderado)
                    printf("-> %d (peso %d) ", aresta_atual->vertice_destino, aresta_atual->peso);
                else
                    printf("-> %d ", aresta_atual->vertice_destino);
                aresta_atual = aresta_atual->proximo;
            }
            printf("-> NULL\n");
        }
    }
}

int main()
{
    int grau_max = 5;
    short int eh_ponderado = 0; // Grafo ponderado
    short int eh_digrafo = 0;   // Grafo não direcionado

    GRAFO *grafo = criar_grafo(grau_max, eh_ponderado, eh_digrafo);

    printf("Grafo criado com sucesso!\n");
    printf("Grau maximo: %d\n", grafo->grau_max);
    printf("Eh ponderado: %d\n", grafo->eh_ponderado);
    printf("Eh digrafo: %d\n", grafo->eh_digrafo);

    // Inserindo arestas no grafo
    inserir_aresta(grafo, 1, 2, 1);
    inserir_aresta(grafo, 1, 3, 1);
    inserir_aresta(grafo, 2, 4, 1);

    printf("\n");
    // Imprimindo a matriz de adjacência do grafo
    imprimir_matriz_grafo(grafo);

    // Removendo uma aresta do grafo
    remover_aresta(grafo, 1, 2);

    printf("\nAresta (1, 2) removida.\n");

    // Imprimindo a matriz de adjacência do grafo após a remoção
    printf("\n");
    imprimir_matriz_grafo(grafo);

    // Libera a memória alocada para o grafo
    liberar_grafo(&grafo);

    return 0;
}