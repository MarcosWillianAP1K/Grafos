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
    int grau; // Grau do vértice
    int peso; // Peso do vértice, s e for ponderado
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
    int n_vertices;                     // Número de vértices do grafo
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
GRAFO iniciar_grafo(short int eh_ponderado, short int eh_digrafo)
{
    GRAFO grafo;

    grafo.eh_ponderado = eh_ponderado;
    grafo.eh_digrafo = eh_digrafo;
    grafo.n_vertices = 0;
    grafo.vetor_adjacencia = NULL;

    return grafo;
}

GRAFO criar_grafo(short int eh_ponderado, short int eh_digrafo, int n_vertices)
{
    GRAFO grafo = iniciar_grafo(eh_ponderado, eh_digrafo);

    grafo.n_vertices = n_vertices;

    grafo.vetor_adjacencia = (VETOR_ADJACENCIA *)malloc(n_vertices * sizeof(VETOR_ADJACENCIA));
    verificar_alocacao(grafo.vetor_adjacencia, "falha ao alocar memória para o vetor de adjacência");

    for (int i = 0; i < n_vertices; i++)
    {
        grafo.vetor_adjacencia[i].vertice.grau = 0;  // Inicializa o grau do vértice
        grafo.vetor_adjacencia[i].vertice.peso = 0;  // Inicializa o peso do vértice, se for ponderado
        grafo.vetor_adjacencia[i].lista_aresta = NULL; // Inicializa a lista de arestas como vazia
    }

    return grafo;
}

// Função para liberar a memória alocada para o grafo
void liberar_grafo(GRAFO *grafo)
{
    if (grafo != NULL)
    {
        for (int i = 0; i < grafo->n_vertices; i++)
        {
            ARESTA *aresta_atual = grafo->vetor_adjacencia[i].lista_aresta;
            while (aresta_atual)
            {
                ARESTA *temp = aresta_atual;
                aresta_atual = aresta_atual->proximo;
                free(temp);
            }
        }
        free(grafo->vetor_adjacencia);
        grafo->vetor_adjacencia = NULL;
        grafo->n_vertices = 0;
    }
}

//=========================ARESTAS==========================

short int inserir_aresta(GRAFO *grafo, int vertice_origem, int vertice_destino, int peso)
{
    short int retorno = 0;

    // Verificações necessarias para inserir a aresta
    if (grafo != NULL && vertice_origem > 0 && vertice_origem <= grafo->n_vertices && vertice_destino > 0 && vertice_destino <= grafo->n_vertices && ((!grafo->eh_ponderado && peso == 1) || (grafo->eh_ponderado)))
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

    if (grafo != NULL && grafo->n_vertices > 0 && vertice_origem > 0 && vertice_origem <= grafo->n_vertices && vertice_destino > 0 && vertice_destino <= grafo->n_vertices)
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

//=========================VERTICES=========================

short int inserir_vertice(GRAFO *grafo, int peso)
{
    short int retorno = 0;

    if (grafo != NULL)
    {
        // Realoca o vetor de adjacência para incluir o novo vértice
        VETOR_ADJACENCIA *novo_vetor = realloc(grafo->vetor_adjacencia, (grafo->n_vertices + 1) * sizeof(VETOR_ADJACENCIA));

        verificar_alocacao(novo_vetor, "falha ao alocar memória para o vetor de adjacência");

        grafo->vetor_adjacencia = novo_vetor;
        grafo->vetor_adjacencia[grafo->n_vertices].vertice.peso = peso;
        grafo->vetor_adjacencia[grafo->n_vertices].vertice.grau = 0;
        grafo->vetor_adjacencia[grafo->n_vertices].lista_aresta = NULL;

        grafo->n_vertices++;
        retorno = 1; // Indica que o vértice foi inserido com sucesso
    }

    return retorno;
}


//Remover ainda possui falhas, ja que o id dos vertices não é atualizado corretamente nas listas de arestas
short int remover_vertice(GRAFO *grafo, int id_vertice)
{
    short int retorno = 0;

    if (grafo != NULL && id_vertice > 0 && id_vertice <= grafo->n_vertices)
    {
        // Remove todas as arestas que apontam para o vértice a ser removido
        if (grafo->eh_digrafo)
        {
            // Se é um dígrafo, remove todas as arestas que apontam para o vértice de destino
            //Tem q passar por todos os vértices e remover as arestas que apontam para o vértice de destino
            for (int i = 0; i < grafo->n_vertices; i++)
            {
                if (i != id_vertice - 1) // Não remover arestas do próprio vértice
                {
                    remover_aresta(grafo, i + 1, id_vertice);
                }
            }
        }
        else
        {
            //Se não é um dígrafo, remove todas as arestas do vértice de origem
            // Percorre a lista de arestas do vértice de origem
            ARESTA *atual = grafo->vetor_adjacencia[id_vertice - 1].lista_aresta;

            while (atual != NULL)
            {
                ARESTA *temp = atual;
                atual = atual->proximo;

                // Remove a aresta do vértice de origem
                remover_aresta(grafo, temp->vertice_destino, id_vertice);
            }
        }

        
        // Libera a memória alocada para as arestas do vértice
        ARESTA *aresta_atual = grafo->vetor_adjacencia[id_vertice - 1].lista_aresta;
        while (aresta_atual != NULL)
        {
            ARESTA *temp = aresta_atual;
            aresta_atual = aresta_atual->proximo;
            free(temp);
        }

        // Realoca o vetor de adjacência para remover o vértice
        for (int i = id_vertice - 1; i < grafo->n_vertices - 1; i++)
        {
            grafo->vetor_adjacencia[i] = grafo->vetor_adjacencia[i + 1];
        }

        grafo->vetor_adjacencia[grafo->n_vertices - 1].lista_aresta = NULL; // Limpa a lista de arestas do último vértice

        VETOR_ADJACENCIA *novo_vetor = realloc(grafo->vetor_adjacencia, (grafo->n_vertices - 1) * sizeof(VETOR_ADJACENCIA));
        verificar_alocacao(novo_vetor, "falha ao alocar memória para o vetor de adjacência");

        grafo->vetor_adjacencia = novo_vetor;
        grafo->n_vertices--;

        retorno = 1; // Indica que o vértice foi removido com sucesso
    }

    return retorno;
}

//=========================IMPRIMIR=========================

void imprimir_lista_grafo(GRAFO *grafo)
{
    if (grafo != NULL)
    {

        printf("\nLista de Adjacencia do Grafo:\n");
        for (int i = 0; i < grafo->n_vertices; i++)
        {
            printf("Vertice %d: ", i + 1);
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
    short int eh_ponderado = 0; // Grafo ponderado
    short int eh_digrafo = 0;   // Grafo não direcionado

    GRAFO grafo = criar_grafo(eh_ponderado, eh_digrafo, 5); // Cria um grafo com 5 vértices

    printf("Grafo criado com sucesso!\n");
    printf("Numero de vertices: %d\n", grafo.n_vertices);
    printf("Eh ponderado: %d\n", grafo.eh_ponderado);
    printf("Eh digrafo: %d\n", grafo.eh_digrafo);


    // Inserindo vértices no grafo
    


    printf("\n");
    // Imprimindo a lista de adjacência do grafo após a inserção dos
    // vértices
    imprimir_lista_grafo(&grafo);

    // Inserindo arestas no grafo
    inserir_aresta(&grafo, 1, 2, 1);
    inserir_aresta(&grafo, 1, 3, 1);
    inserir_aresta(&grafo, 2, 4, 1);

    printf("\n");
    // Imprimindo a lista de adjacência do grafo
    imprimir_lista_grafo(&grafo);

    // Removendo uma aresta do grafo
    remover_aresta(&grafo, 1, 2);

    printf("\nAresta (1, 2) removida.\n");

    // Imprimindo a lista de adjacência do grafo após a remoção
    printf("\n");
    imprimir_lista_grafo(&grafo);

    

    // Imprimindo a lista de adjacência do grafo após a remoção do vértice
    printf("\n");
    imprimir_lista_grafo(&grafo);

    // Libera a memória alocada para o grafo
    liberar_grafo(&grafo);

    return 0;
}