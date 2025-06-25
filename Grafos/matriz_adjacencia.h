
#pragma once

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



void liberar_grafo(GRAFO *grafo);

GRAFO iniciar_grafo(short int eh_ponderado, short int eh_digrafo);

GRAFO criar_grafo(short int eh_ponderado, short int eh_digrafo, int n_vertices);

short int criar_aresta(GRAFO *grafo, int vertice_origem, int vertice_destino, int peso);

short int apagar_aresta(GRAFO *grafo, int vertice_origem, int vertice_destino);

short int criar_vertice(GRAFO *grafo, int peso);

short int apagar_vertice(GRAFO *grafo, int id_vertice);

void imprimir_matriz_grafo(GRAFO *grafo);