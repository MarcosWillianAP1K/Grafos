#include <limits.h> // Para INT_MAX
#include <stdlib.h> // Para malloc e free
#include <stdio.h>  // Para printf, se necessário
#include "Dijkstra.h" // Inclua o cabeçalho apropriado para a estrutura VETOR_DIJKSTRA
#include "../Utilitarios/Utilitarios.h" // Inclua o cabeçalho para verificar alocação
#include "../Grafos/matriz_adjacencia.h" // Inclua o cabeçalho para MATRIZ_ADJACENCIA, se necessário







void inicializar_vetor_dijkstra(VETOR_DIJKSTRA *vetor, int n_vertices)
{
    for (int i = 0; i < n_vertices; i++)
    {
        vetor[i].vertice_anterior = -1; // Inicializa o vértice anterior como -1 (nenhum)
        vetor[i].vertice_atual = -1;    // Inicializa o vértice atual como -1 (nenhum)
        vetor[i].peso = INT_MAX;          // Inicializa o peso como infinito
    }
}

VETOR_DIJKSTRA *alocar_vetor_dijkstra(int n_vertices)
{
    VETOR_DIJKSTRA *vetor = (VETOR_DIJKSTRA *)malloc(n_vertices * sizeof(VETOR_DIJKSTRA));
    verificar_alocacao(vetor, "falha ao alocar memória para o vetor de Dijkstra");
    inicializar_vetor_dijkstra(vetor, n_vertices);

    return vetor;
}


short int dijkstra(GRAFO *grafo, int vertice_inicial)
{
    short int retorno = 0;

    if (grafo != NULL && grafo->n_vertices > 0 && vertice_inicial > 0 && vertice_inicial <= grafo->n_vertices)
    {
        VETOR_DIJKSTRA *vetor_vertices = alocar_vetor_dijkstra(grafo->n_vertices);

        vetor_vertices[vertice_inicial - 1].distancia = 0; // A distância do vértice inicial é 0
    }

    return retorno;
}
















void imprimir_vetor_dijkstra(VETOR_DIJKSTRA *vetor, int n_vertices)
{
    printf("Vetor Dijkstra:\n");
    for (int i = 0; i < n_vertices; i++)
    {
        printf("Vértice %d: Anterior: %p, Atual: %p, Peso: %d\n", i, vetor[i].vertice_anterior, vetor[i].vertice_atual, vetor[i].peso);
    }
}