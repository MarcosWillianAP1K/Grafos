#include <limits.h>                      // Para INT_MAX
#include <stdlib.h>                      // Para malloc e free
#include <stdio.h>                       // Para printf, se necessário
#include "Dijkstra.h"                    // Inclua o cabeçalho apropriado para a estrutura VETOR_DIJKSTRA
#include "../Utilitarios/Utilitarios.h"  // Inclua o cabeçalho para verificar alocação




void liberar_vetor_dijkstra(VETOR_DIJKSTRA **vetor)
{
    if (vetor != NULL)
    {
        free(*vetor); // Libera a memória alocada para o vetor de Dijkstra
        *vetor = NULL; // Define o ponteiro como NULL para evitar dangling pointer
    }
}

void inicializar_vetor_dijkstra(VETOR_DIJKSTRA *vetor, int n_vertices)
{
    for (int i = 0; i < n_vertices; i++)
    {
        vetor[i].vertice_anterior = -1; // Inicializa o vértice anterior como -1 (nenhum)
        vetor[i].vertice_visitado = 0;    // Inicializa o vértice visitado como 0 (não visitado)
        vetor[i].distancia = INT_MAX;        // Inicializa a distância como infinito
    }
}

VETOR_DIJKSTRA *alocar_vetor_dijkstra(int n_vertices)
{
    VETOR_DIJKSTRA *vetor = (VETOR_DIJKSTRA *)malloc(n_vertices * sizeof(VETOR_DIJKSTRA));
    verificar_alocacao(vetor, "falha ao alocar memória para o vetor de Dijkstra");
    inicializar_vetor_dijkstra(vetor, n_vertices);

    return vetor;
}

VETOR_DIJKSTRA *dijkstra(GRAFO *grafo, int vertice_inicial)
{
    VETOR_DIJKSTRA *vetor_vertices = NULL;

    if (grafo != NULL && grafo->n_vertices > 0 && vertice_inicial > 0 && vertice_inicial <= grafo->n_vertices)
    {
        vetor_vertices = alocar_vetor_dijkstra(grafo->n_vertices);

        vetor_vertices[vertice_inicial - 1].distancia = 0; // A distância do vértice inicial é 0
    }

    // Encontra o caminho mais curto para todos os vértices
    for (int qtdRelaxamentos = 0; qtdRelaxamentos < grafo->n_vertices - 1; qtdRelaxamentos++)
    {
        int distanciaMinima = INT_MAX, indiceVerticeAtual = 0;

        // Encontra o próximo vértice não visitado com a menor distância
        for (int i = 0; i < grafo->n_vertices; i++)
        {
            if ((!vetor_vertices[i].vertice_visitado && vetor_vertices[i].distancia <= distanciaMinima))
            {
                distanciaMinima = vetor_vertices[i].distancia;
                indiceVerticeAtual = i;
            }
        }

        // Marca o próximo vértice como visitado
        vetor_vertices[indiceVerticeAtual].vertice_visitado = 1;

        // Atualiza as distâncias dos vértices adjacentes não visitados
        for (int i = 0; i < grafo->n_vertices; i++)
        {
            if (!vetor_vertices[i].vertice_visitado && grafo->matriz_adjacencia[indiceVerticeAtual][i].bolean)
            {
                int dist = vetor_vertices[indiceVerticeAtual].distancia + grafo->matriz_adjacencia[indiceVerticeAtual][i].peso_aresta;

                if (dist < vetor_vertices[i].distancia)
                {
                    vetor_vertices[i].distancia = dist;
                    vetor_vertices[i].vertice_anterior = indiceVerticeAtual;
                }
            }
        }
    }

    return vetor_vertices;
}

void imprimir_vetor_dijkstra(VETOR_DIJKSTRA *vetor, int n_vertices)
{
    printf("Vetor Dijkstra:\n");
    for (int i = 0; i < n_vertices; i++)
    {
        if (vetor[i].distancia == INT_MAX)
        {
            printf("Vertice %d: Anterior: %d, Visitado: %d, distancia: Infinito\n", i + 1, vetor[i].vertice_anterior + 1, vetor[i].vertice_visitado);
        }
        else
            printf("Vertice %d: Anterior: %d, Visitado: %d, distancia: %d\n", i + 1, vetor[i].vertice_anterior + 1, vetor[i].vertice_visitado, vetor[i].distancia);
    }
}