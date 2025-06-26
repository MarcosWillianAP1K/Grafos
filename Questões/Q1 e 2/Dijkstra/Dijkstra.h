#include "../Grafos/matriz_adjacencia.h" // Inclua o cabeçalho para MATRIZ_ADJACENCIA, se necessário

#pragma once

typedef struct VETOR_DIJKSTRA
{
    int vertice_anterior; // Vértice anterior no caminho mais curto
    short int vertice_visitado;    // Indica se o vértice já foi visitado
    int distancia;        // Distância do vértice
} VETOR_DIJKSTRA;

void liberar_vetor_dijkstra(VETOR_DIJKSTRA **vetor);

void inicializar_vetor_dijkstra(VETOR_DIJKSTRA *vetor, int n_vertices);

VETOR_DIJKSTRA *alocar_vetor_dijkstra(int n_vertices);

VETOR_DIJKSTRA *dijkstra(GRAFO *grafo, int vertice_inicial);

void imprimir_vetor_dijkstra(VETOR_DIJKSTRA *vetor, int n_vertices);