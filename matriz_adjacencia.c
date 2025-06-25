#include <stdio.h>
#include <stdlib.h>

typedef struct MATRIZ_ADJACENCIA
{
    short int bolean;
    int peso_aresta; // Peso da aresta, se for ponderada

} MATRIZ_ADJACENCIA;

typedef struct VERTICE
{
    int grau; // Grau do vértice
    int peso; // Peso do vértice, se for ponderado
} VERTICE;

typedef struct GRAFO
{
    short int eh_ponderado;                // 1 se o grafo é ponderado, 0 se não
    short int eh_digrafo;                  // 1 se o grafo é um dígrafo, 0 se não
    int n_vertices;                        // Número de vértices do grafo
    MATRIZ_ADJACENCIA **matriz_adjacencia; // Matriz de adjacência
    VERTICE *vetor_vertices;               // Vetor de vértices
} GRAFO;

void verificar_alocacao(void *ptr, const char *mensagem)
{
    if (!ptr)
    {
        fprintf(stderr, "Erro de alocacao: %s\n", mensagem);
        exit(EXIT_FAILURE);
    }
}

// Função para criar um grafo com grau máximo, ponderado e direcionado
GRAFO iniciar_grafo(short int eh_ponderado, short int eh_digrafo)
{
    GRAFO grafo;

    grafo.eh_ponderado = eh_ponderado;
    grafo.eh_digrafo = eh_digrafo;
    grafo.n_vertices = 0;
    grafo.matriz_adjacencia = NULL;
    grafo.vetor_vertices = NULL;

    return grafo;
}

void liberar_grafo(GRAFO *grafo)
{
    if (grafo != NULL)
    {
        if (grafo->matriz_adjacencia != NULL)
        {
            for (int i = 0; i < grafo->n_vertices; i++)
            {
                free(grafo->matriz_adjacencia[i]);
            }
            free(grafo->matriz_adjacencia);
        }
        grafo->matriz_adjacencia = NULL;

        if (grafo->vetor_vertices != NULL)
        {
            free(grafo->vetor_vertices);
        }
        grafo->vetor_vertices = NULL;

        grafo->n_vertices = 0;
    }
}

//=========================ARESTAS==========================

short int criar_aresta(GRAFO *grafo, int vertice_origem, int vertice_destino, int peso)
{
    short int retorno = 0;

    // Verificações necessarias para inserir a aresta
    if (grafo != NULL && vertice_origem > 0 && vertice_origem <= grafo->n_vertices && vertice_destino > 0 && vertice_destino <= grafo->n_vertices && ((!grafo->eh_ponderado && peso == 1) || (grafo->eh_ponderado)))
    {
        grafo->matriz_adjacencia[vertice_origem - 1][vertice_destino - 1].bolean = 1;         // Marca a aresta como existente
        grafo->matriz_adjacencia[vertice_origem - 1][vertice_destino - 1].peso_aresta = peso; // Define o peso da aresta, se for ponderada
        grafo->vetor_vertices[vertice_origem - 1].grau++;                                     // Incrementa o grau do vértice de origem

        if (!grafo->eh_digrafo)
        {
            // Se o grafo não é direcionado, adiciona a aresta na direção oposta
            grafo->matriz_adjacencia[vertice_destino - 1][vertice_origem - 1].bolean = 1;         // Marca a aresta como existente
            grafo->matriz_adjacencia[vertice_destino - 1][vertice_origem - 1].peso_aresta = peso; // Define o peso da aresta, se for ponderada
            grafo->vetor_vertices[vertice_destino - 1].grau++;                                    // Incrementa o grau do vértice de destino
        }

        retorno = 1; // Indica que a aresta foi inserida com sucesso
    }

    return retorno;
}

short int apagar_aresta(GRAFO *grafo, int vertice_origem, int vertice_destino)
{
    short int retorno = 0;

    if (grafo != NULL && grafo->n_vertices > 0 && vertice_origem > 0 && vertice_origem <= grafo->n_vertices && vertice_destino > 0 && vertice_destino <= grafo->n_vertices)
    {

        grafo->matriz_adjacencia[vertice_origem - 1][vertice_destino - 1].bolean = 0;      // Marca a aresta como inexistente
        grafo->matriz_adjacencia[vertice_origem - 1][vertice_destino - 1].peso_aresta = 0; // Zera o peso da aresta, se for ponderada
        grafo->vetor_vertices[vertice_origem - 1].grau--;                                  // Decrementa o grau do vértice de origem

        if (!grafo->eh_digrafo)
        {
            // Se o grafo não é direcionado, remove a aresta na direção oposta
            grafo->matriz_adjacencia[vertice_destino - 1][vertice_origem - 1].bolean = 0;      // Marca a aresta como inexistente
            grafo->matriz_adjacencia[vertice_destino - 1][vertice_origem - 1].peso_aresta = 0; // Zera o peso da aresta, se for ponderada
            grafo->vetor_vertices[vertice_destino - 1].grau--;                                 // Decrementa o grau do vértice de destino
        }

        retorno = 1; // Indica que a aresta foi removida com sucesso
    }

    return retorno;
}

//=========================VERTICES=========================

short int criar_vertice(GRAFO *grafo, int peso)
{
    short int retorno = 0;

    if (grafo != NULL)
    {
        int novo_n_vertices = grafo->n_vertices + 1;

        // Realoca o vetor de vértices para adicionar um novo vértice
        VERTICE *novo_vetor = realloc(grafo->vetor_vertices, novo_n_vertices * sizeof(VERTICE));
        verificar_alocacao(novo_vetor, "falha ao alocar memória para o vetor de vértices");

        grafo->vetor_vertices = novo_vetor;

        // Inicializa o novo vértice
        grafo->vetor_vertices[grafo->n_vertices].grau = 0;    // Grau inicial do vértice
        grafo->vetor_vertices[grafo->n_vertices].peso = peso; // Peso do vértice, se for ponderado

        // Realoca a matriz de adjacência para adicionar uma nova linha
        MATRIZ_ADJACENCIA **nova_matriz = realloc(grafo->matriz_adjacencia, novo_n_vertices * sizeof(MATRIZ_ADJACENCIA *));
        verificar_alocacao(nova_matriz, "falha ao alocar memória para a matriz de adjacência");

        grafo->matriz_adjacencia = nova_matriz;

        // Expande cada linha existente para adicionar uma nova coluna
        for (int i = 0; i < grafo->n_vertices; i++)
        {
            MATRIZ_ADJACENCIA *linha_expandida = realloc(grafo->matriz_adjacencia[i], novo_n_vertices * sizeof(MATRIZ_ADJACENCIA));
            verificar_alocacao(linha_expandida, "falha ao expandir linha da matriz de adjacência");

            grafo->matriz_adjacencia[i] = linha_expandida;

            // Inicializa a nova célula na coluna adicionada
            grafo->matriz_adjacencia[i][grafo->n_vertices].bolean = 0;
            grafo->matriz_adjacencia[i][grafo->n_vertices].peso_aresta = 0;
        }

        // Aloca e inicializa a nova linha inteira
        grafo->matriz_adjacencia[grafo->n_vertices] = malloc(novo_n_vertices * sizeof(MATRIZ_ADJACENCIA));
        verificar_alocacao(grafo->matriz_adjacencia[grafo->n_vertices],"falha ao alocar memória para a nova linha da matriz de adjacência");

        // Inicializa toda a nova linha
        for (int j = 0; j < novo_n_vertices; j++)
        {
            grafo->matriz_adjacencia[grafo->n_vertices][j].bolean = 0;
            grafo->matriz_adjacencia[grafo->n_vertices][j].peso_aresta = 0;
        }

        grafo->n_vertices++; // Incrementa o número de vértices
        retorno = 1;         // Indica que o vértice foi inserido com sucesso
    }

    return retorno;
}

// Remover ainda possui falhas, ja que o id dos vertices não é atualizado corretamente nas listas de arestas
short int apagar_vertice(GRAFO *grafo, int id_vertice)
{
    short int retorno = 0;

    if (grafo != NULL && id_vertice > 0 && id_vertice <= grafo->n_vertices)
    {
        // Remove todas as arestas que apontam para o vértice a ser removido
        if (!grafo->eh_digrafo)
        {
            for (int i = 0; i < grafo->n_vertices; i++)
            {
                if (i != id_vertice - 1) // Não remover arestas do próprio vértice
                {
                    apagar_aresta(grafo, i + 1, id_vertice);
                }
            }
        }

        // Libera a linha correspondente ao vértice
        free(grafo->matriz_adjacencia[id_vertice - 1]); // Libera a linha correspondente ao vértice
        grafo->matriz_adjacencia[id_vertice - 1] = NULL;

        for (int i = 0; i < grafo->n_vertices - 1; i++)
        {
            if (i >= id_vertice - 1)
            {
                grafo->matriz_adjacencia[i] = grafo->matriz_adjacencia[i + 1]; // Move as linhas para cima
                grafo->vetor_vertices[i] = grafo->vetor_vertices[i + 1];       // Move os vértices para cima
            }

            // Remove a coluna correspondente ao vértice
            for (int j = id_vertice - 1; j < grafo->n_vertices - 1; j++)
            {
                grafo->matriz_adjacencia[i][j] = grafo->matriz_adjacencia[i][j + 1];
            }

            MATRIZ_ADJACENCIA *nova_coluna = realloc(grafo->matriz_adjacencia[i], (grafo->n_vertices - 1) * sizeof(MATRIZ_ADJACENCIA));
            verificar_alocacao(nova_coluna, "falha ao alocar memória para a nova coluna da matriz de adjacência");
            grafo->matriz_adjacencia[i] = nova_coluna;
        }

        // Realoca o vetor de vértices para remover o vértice
        MATRIZ_ADJACENCIA **nova_matriz = realloc(grafo->matriz_adjacencia, (grafo->n_vertices - 1) * sizeof(MATRIZ_ADJACENCIA *));
        verificar_alocacao(nova_matriz, "falha ao alocar memória para a matriz de adjacência");

        grafo->matriz_adjacencia = nova_matriz;

        VERTICE *novo_vetor = realloc(grafo->vetor_vertices, (grafo->n_vertices - 1) * sizeof(VERTICE));
        verificar_alocacao(novo_vetor, "falha ao alocar memória para o vetor de vértices");

        grafo->vetor_vertices = novo_vetor;

        grafo->n_vertices--; // Decrementa o número de vértices
        retorno = 1;         // Indica que o vértice foi removido com sucesso
    }

    return retorno;
}

//=========================IMPRIMIR=========================

void imprimir_matriz_grafo(GRAFO *grafo)
{
    if (grafo != NULL)
    {
        printf("Matriz de Adjacencia:\n");
        for (int i = -2; i < grafo->n_vertices; i++)
        {

            if (i < 0)
            {
                printf("   ");
                for (int j = 0; j < grafo->n_vertices; j++)
                {
                    if (i == -1)
                    {
                        printf("__");
                    }
                    else
                    {
                        printf("%d ", j + 1); // Imprime os índices das colunas
                    }
                }
            }
            else
            {
                printf("%d| ", i + 1); // Imprime o índice do vértice
                for (int j = 0; j < grafo->n_vertices; j++)
                {
                    if (i == -1)
                    {
                        printf("%d ", j + 1); // Imprime os índices das colunas
                    }
                    else
                    {
                        printf("%d ", grafo->matriz_adjacencia[i][j].bolean); // Imprime se a aresta existe (1) ou não (0)
                    }
                }
            }

            printf("\n");
        }
    }
    else
    {
        printf("Grafo não inicializado.\n");
    }
}

int main()
{
    short int eh_ponderado = 0; // Grafo ponderado
    short int eh_digrafo = 0;   // Grafo não direcionado

    GRAFO grafo = iniciar_grafo(eh_ponderado, eh_digrafo);

    printf("Grafo criado com sucesso!\n");
    printf("Numero de vertices: %d\n", grafo.n_vertices);
    printf("Eh ponderado: %d\n", grafo.eh_ponderado);
    printf("Eh digrafo: %d\n", grafo.eh_digrafo);

    // Inserindo vértices no grafo
    criar_vertice(&grafo, 0); // Vértice 1
    criar_vertice(&grafo, 0); // Vértice 2
    criar_vertice(&grafo, 0); // Vértice 3
    criar_vertice(&grafo, 0); // Vértice 4
    criar_vertice(&grafo, 0); // Vértice 5

    printf("\n");
    // Imprimindo a matriz de adjacência do grafo após a inserção dos vértices
    imprimir_matriz_grafo(&grafo);


    criar_aresta(&grafo, 1, 2, 1);
    criar_aresta(&grafo, 1, 3, 1);
    criar_aresta(&grafo, 2, 4, 1);

    printf("\n");
    // Imprimindo a matriz de adjacência do grafo após a inserção das
    // arestas
    imprimir_matriz_grafo(&grafo);

    // Removendo arestas do grafo
    apagar_aresta(&grafo, 1, 2);

    printf("\nAresta (1, 2) removida.\n");

    // Imprimindo a matriz de adjacência do grafo após a remoção
    printf("\n");
    imprimir_matriz_grafo(&grafo);


    // Removendo um vértice do grafo
    apagar_vertice(&grafo, 3);
    printf("\nVertice 3 removido.\n");

    // Imprimindo a matriz de adjacência do grafo após a remoção do vértice
    printf("\n");
    imprimir_matriz_grafo(&grafo);


    liberar_grafo(&grafo); // Libera a memória alocada para o grafo
    printf("\n\nGrafo liberado com sucesso!\n");

    return 0;
}