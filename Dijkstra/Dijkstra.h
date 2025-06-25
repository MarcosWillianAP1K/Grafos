
#pragma once

typedef struct VETOR_DIJKSTRA
{
    int vertice_anterior; // Vértice anterior no caminho mais curto
    int vertice_atual;    // Vértice atual no caminho mais curto
    int distancia;        // Distância do vértice
} VETOR_DIJKSTRA;