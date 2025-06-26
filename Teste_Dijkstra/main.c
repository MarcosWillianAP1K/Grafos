#include <stdio.h>
#include <stdlib.h>
#include "../Dijkstra/Dijkstra.h"


//gcc main.c ../Dijkstra/Dijkstra.c ../Grafos/matriz_adjacencia.c ../Utilitarios/Utilitarios.c

int main()
{
    printf("=== Teste Dijkstra ===\n");

    GRAFO grafo = criar_grafo(0, 1, 5); // Exemplo de grafo com 5 vértices, não ponderado e não dígrafo

    criar_aresta(&grafo, 1, 2, 1);
    // criar_aresta(&grafo, 1, 4, 1);
    criar_aresta(&grafo, 1, 5, 1);
    criar_aresta(&grafo, 2, 3, 1);
    criar_aresta(&grafo, 3, 4, 1);
    criar_aresta(&grafo, 5, 4, 1);

    VETOR_DIJKSTRA *resultado = dijkstra(&grafo, 4);

    if (resultado != NULL)
    {
        imprimir_vetor_dijkstra(resultado, grafo.n_vertices);
    }
    else
    {
        printf("Erro ao executar Dijkstra.\n");
    }

    liberar_vetor_dijkstra(&resultado);

    return 0;
}