#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_DISCOS 4
#define NUM_PINOS 3
#define NUM_CONFIGURACOES 81 // 3^4 = 81 configuracoes possiveis
#define INFINITO 999999      // Valor grande para representar infinito

// Estrutura para representar uma configuracao
typedef struct
{
    int discos[NUM_DISCOS]; // disco[i] = pino onde esta o disco i
} Configuracao;

// Estrutura para o algoritmo de Dijkstra
typedef struct
{
    int distancia;
    int visitado;
    int anterior;
} NoGrafo;

// Funcao para converter configuracao em indice unico
int configuracao_para_indice(Configuracao config)
{
    int indice = 0;
    int multiplicador = 1;

    for (int i = 0; i < NUM_DISCOS; i++)
    {
        indice += config.discos[i] * multiplicador;
        multiplicador *= NUM_PINOS;
    }

    return indice;
}

// Funcao para converter indice em configuracao
Configuracao indice_para_configuracao(int indice)
{
    Configuracao config;

    for (int i = 0; i < NUM_DISCOS; i++)
    {
        config.discos[i] = indice % NUM_PINOS;
        indice /= NUM_PINOS;
    }

    return config;
}

// Funcao para verificar se um movimento e valido
int movimento_valido(Configuracao origem, Configuracao destino)
{
    int diferencas = 0;
    int disco_movido = -1;
    int pino_origem = -1, pino_destino = -1;
    int resultado = 0;
    int i;

    // Encontrar qual disco foi movido
    for (i = 0; i < NUM_DISCOS && diferencas <= 1; i++)
    {
        if (origem.discos[i] != destino.discos[i])
        {
            diferencas++;
            if (diferencas <= 1)
            {
                disco_movido = i;
                pino_origem = origem.discos[i];
                pino_destino = destino.discos[i];
            }
        }
    }

    if (diferencas == 1)
    {
        resultado = 1; // Assume valido inicialmente

        // Verificar se o disco movido estava no topo do pino de origem
        for (i = disco_movido + 1; i < NUM_DISCOS && resultado == 1; i++)
        {
            if (origem.discos[i] == pino_origem)
            {
                resultado = 0; // Ha um disco menor em cima
            }
        }

        // Verificar se o disco pode ser colocado no pino de destino
        for (i = disco_movido + 1; i < NUM_DISCOS && resultado == 1; i++)
        {
            if (destino.discos[i] == pino_destino)
            {
                resultado = 0; // Ha um disco menor no pino de destino
            }
        }
    }
    else
    {
        resultado = 0; // Nenhum ou mais de um disco movido
    }

    return resultado;
}

// Funcao para criar a matriz de adjacencia
void criar_matriz_adjacencia(int matriz[NUM_CONFIGURACOES][NUM_CONFIGURACOES])
{
    printf("Criando matriz de adjacencia...\n");

    // Inicializar matriz
    for (int i = 0; i < NUM_CONFIGURACOES; i++)
    {
        for (int j = 0; j < NUM_CONFIGURACOES; j++)
        {
            matriz[i][j] = 0;
        }
    }

    // Preencher matriz verificando movimentos válidos
    for (int i = 0; i < NUM_CONFIGURACOES; i++)
    {
        Configuracao config1 = indice_para_configuracao(i);

        for (int j = 0; j < NUM_CONFIGURACOES; j++)
        {
            if (i != j)
            {
                Configuracao config2 = indice_para_configuracao(j);

                if (movimento_valido(config1, config2))
                {
                    matriz[i][j] = 1; // Peso 1 para cada aresta
                }
            }
        }
    }

    printf("Matriz de adjacencia criada!\n");
}

// Funcao para encontrar o vertice com menor distancia nao visitado
int encontrar_menor_distancia(NoGrafo nos[NUM_CONFIGURACOES])
{
    int menor_dist = INFINITO;
    int menor_indice = -1;

    for (int i = 0; i < NUM_CONFIGURACOES; i++)
    {
        if (!nos[i].visitado && nos[i].distancia < menor_dist)
        {
            menor_dist = nos[i].distancia;
            menor_indice = i;
        }
    }

    return menor_indice;
}

// Algoritmo de Dijkstra
int dijkstra(int matriz[NUM_CONFIGURACOES][NUM_CONFIGURACOES], int origem, int destino, int caminho[NUM_CONFIGURACOES])
{
    NoGrafo nos[NUM_CONFIGURACOES];
    int tamanho_caminho = 0;
    int continuar = 1;
    int count;

    // Inicializacao
    for (int i = 0; i < NUM_CONFIGURACOES; i++)
    {
        nos[i].distancia = INFINITO;
        nos[i].visitado = 0;
        nos[i].anterior = -1;
    }

    nos[origem].distancia = 0;

    // Algoritmo principal
    for (count = 0; count < NUM_CONFIGURACOES && continuar; count++)
    {
        int u = encontrar_menor_distancia(nos);

        if (u == -1 || nos[u].distancia == INFINITO)
        {
            continuar = 0;
        }
        else
        {
            nos[u].visitado = 1;

            if (u == destino)
            {
                continuar = 0; // Chegou ao destino
            }
            else
            {
                // Atualizar distancias dos vizinhos
                for (int v = 0; v < NUM_CONFIGURACOES; v++)
                {
                    if (!nos[v].visitado && matriz[u][v] > 0)
                    {
                        int nova_distancia = nos[u].distancia + matriz[u][v];

                        if (nova_distancia < nos[v].distancia)
                        {
                            nos[v].distancia = nova_distancia;
                            nos[v].anterior = u;
                        }
                    }
                }
            }
        }
    }

    // Reconstruir caminho
    if (nos[destino].distancia != INFINITO)
    {
        int atual = destino;
        while (atual != -1)
        {
            caminho[tamanho_caminho++] = atual;
            atual = nos[atual].anterior;
        }

        // Inverter caminho
        for (int i = 0; i < tamanho_caminho / 2; i++)
        {
            int temp = caminho[i];
            caminho[i] = caminho[tamanho_caminho - 1 - i];
            caminho[tamanho_caminho - 1 - i] = temp;
        }
    }

    return tamanho_caminho;
}

// Funcao para imprimir configuracao
void imprimir_configuracao(Configuracao config)
{
    printf("Configuracao: ");
    for (int i = 0; i < NUM_DISCOS; i++)
    {
        printf("D%d->P%d ", i + 1, config.discos[i] + 1);
    }
    printf("\n");
}

// Funcao para imprimir o estado visual dos pinos
void imprimir_estado_visual(Configuracao config)
{
    printf("\nEstado visual dos pinos:\n");

    // Criar representacao dos pinos
    int pinos[NUM_PINOS][NUM_DISCOS];
    int topo[NUM_PINOS] = {0, 0, 0};

    // Limpar pinos
    for (int i = 0; i < NUM_PINOS; i++)
    {
        for (int j = 0; j < NUM_DISCOS; j++)
        {
            pinos[i][j] = -1;
        }
    }

    // Colocar discos nos pinos (do maior para o menor)
    for (int disco = NUM_DISCOS - 1; disco >= 0; disco--)
    {
        int pino = config.discos[disco];
        pinos[pino][topo[pino]++] = disco + 1;
    }

    // Imprimir de cima para baixo
    for (int nivel = NUM_DISCOS - 1; nivel >= 0; nivel--)
    {
        for (int pino = 0; pino < NUM_PINOS; pino++)
        {
            if (pinos[pino][nivel] != -1)
            {
                printf("  D%d  ", pinos[pino][nivel]);
            }
            else
            {
                printf("  |   ");
            }
        }
        printf("\n");
    }

    // Base dos pinos
    for (int pino = 0; pino < NUM_PINOS; pino++)
    {
        printf(" --- ");
    }
    printf("\n");

    // Numeros dos pinos
    for (int pino = 0; pino < NUM_PINOS; pino++)
    {
        printf(" P%d  ", pino + 1);
    }
    printf("\n\n");
}

// Funcao para mostrar a matriz de adjacencia
void mostrar_matriz_adjacencia(int matriz[NUM_CONFIGURACOES][NUM_CONFIGURACOES])
{
    printf("\n=== MATRIZ DE ADJACENCIA %dx%d ===\n", NUM_CONFIGURACOES, NUM_CONFIGURACOES);

    // Cabecalho com indices das colunas
    printf("    ");
    for (int j = 0; j < NUM_CONFIGURACOES; j++)
    {
        printf("%2d", j);
        if (j < NUM_CONFIGURACOES - 1)
            printf(" ");
    }
    printf("\n");

    // Linha separadora
    printf("   ");
    for (int j = 0; j < NUM_CONFIGURACOES; j++)
    {
        printf("---");
    }
    printf("\n");

    // Mostrar a matriz completa
    for (int i = 0; i < NUM_CONFIGURACOES; i++)
    {
        printf("%2d|", i); // Indice da linha

        for (int j = 0; j < NUM_CONFIGURACOES; j++)
        {
            printf("%2d", matriz[i][j]);
            if (j < NUM_CONFIGURACOES - 1)
                printf(" ");
        }
        printf("\n");
    }

    // Contar total de conexoes
    int total_conexoes = 0;
    for (int i = 0; i < NUM_CONFIGURACOES; i++)
    {
        for (int j = 0; j < NUM_CONFIGURACOES; j++)
        {
            if (matriz[i][j] == 1)
            {
                total_conexoes++;
            }
        }
    }

    printf("\nEstatisticas da matriz:\n");
    printf("Tamanho: %dx%d\n", NUM_CONFIGURACOES, NUM_CONFIGURACOES);
    printf("Total de conexoes (arestas): %d\n", total_conexoes);
    printf("Total de elementos: %d\n", NUM_CONFIGURACOES * NUM_CONFIGURACOES);
}

// Funcao para mostrar apenas as conexoes da matriz
void mostrar_conexoes(int matriz[NUM_CONFIGURACOES][NUM_CONFIGURACOES])
{
    printf("\n=== CONEXOES DA MATRIZ DE ADJACENCIA ===\n");
    printf("Mostrando apenas conexoes existentes (valor 1):\n\n");

    int conexoes_mostradas = 0;
    for (int i = 0; i < NUM_CONFIGURACOES; i++)
    {
        for (int j = 0; j < NUM_CONFIGURACOES; j++)
        {
            if (matriz[i][j] == 1)
            {
                Configuracao config_i = indice_para_configuracao(i);
                Configuracao config_j = indice_para_configuracao(j);

                printf("Conexao %d: [", conexoes_mostradas + 1);
                for (int k = 0; k < NUM_DISCOS; k++)
                {
                    printf("%d", config_i.discos[k]);
                    if (k < NUM_DISCOS - 1)
                        printf(",");
                }
                printf("] -> [");
                for (int k = 0; k < NUM_DISCOS; k++)
                {
                    printf("%d", config_j.discos[k]);
                    if (k < NUM_DISCOS - 1)
                        printf(",");
                }
                printf("] (indice %d -> %d)\n", i, j);

                conexoes_mostradas++;
            }
        }
    }

    printf("\nTotal de conexoes encontradas: %d\n", conexoes_mostradas);
}

// Funcao para mostrar o menu
int mostrar_menu()
{
    int opcao;

    printf("\n=== MENU TORRE DE HANOI ===\n");
    printf("1. Criar matriz de adjacencia\n");
    printf("2. Mostrar matriz de adjacencia (81x81)\n");
    printf("3. Mostrar conexoes da matriz\n");
    printf("4. Encontrar menor caminho (Dijkstra)\n");
    printf("5. Mostrar configuracao inicial e final\n");
    printf("6. Executar tudo (opcao completa)\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");

    scanf("%d", &opcao);
    getchar(); // Limpar o buffer

    return opcao;
}

int main()
{
    printf("=== TORRE DE HANOI COM 4 DISCOS ===\n");

    int resultado = 0;

    // Alocar matriz de adjacencia
    int (*matriz)[NUM_CONFIGURACOES] = malloc(NUM_CONFIGURACOES * NUM_CONFIGURACOES * sizeof(int));
    if (!matriz)
    {
        printf("Erro ao alocar memoria!\n");
        resultado = 1;
    }
    else
    {
        // Configuracao inicial: todos os discos no pino 0
        Configuracao inicial = {{0, 0, 0, 0}};

        // Configuracao final: todos os discos no pino 2
        Configuracao final = {{2, 2, 2, 2}};

        int matriz_criada = 0;
        int opcao;

        do
        {
            opcao = mostrar_menu();

            switch (opcao)
            {
            case 1:
            {
                if (!matriz_criada)
                {
                    printf("\nCriando matriz de adjacencia...\n");
                    clock_t inicio = clock();
                    criar_matriz_adjacencia(matriz);
                    clock_t fim = clock();
                    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                    printf("Matriz criada com sucesso!\n");
                    printf("Tempo gasto: %.6f segundos\n", tempo);
                    matriz_criada = 1;
                }
                else
                {
                    printf("Matriz ja foi criada!\n");
                }
                break;
            }

            case 2:
            {
                if (matriz_criada)
                {
                    mostrar_matriz_adjacencia(matriz);
                }
                else
                {
                    printf("Erro: Matriz ainda nao foi criada! Use a opcao 1 primeiro.\n");
                }
                break;
            }

            case 3:
            {
                if (matriz_criada)
                {
                    mostrar_conexoes(matriz);
                }
                else
                {
                    printf("Erro: Matriz ainda nao foi criada! Use a opcao 1 primeiro.\n");
                }
                break;
            }

            case 4:
            {
                if (matriz_criada)
                {
                    printf("\nEncontrando menor caminho usando Dijkstra...\n");

                    int indice_inicial = configuracao_para_indice(inicial);
                    int indice_final = configuracao_para_indice(final);
                    int caminho[NUM_CONFIGURACOES];

                    clock_t inicio = clock();
                    int tamanho_caminho = dijkstra(matriz, indice_inicial, indice_final, caminho);
                    clock_t fim = clock();

                    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

                    if (tamanho_caminho > 0)
                    {
                        printf("\nSolucao encontrada!\n");
                        printf("Numero de movimentos: %d\n", tamanho_caminho - 1);
                        printf("Tempo do algoritmo: %.6f segundos\n", tempo);

                        // Verificar se e otimo
                        int min_teorico = (1 << NUM_DISCOS) - 1;
                        printf("Numero minimo teorico: %d movimentos\n", min_teorico);

                        if (tamanho_caminho - 1 == min_teorico)
                        {
                            printf("✓ Solucao otima encontrada!\n");
                        }

                        printf("\nDeseja ver o caminho completo? (s/n): ");
                        char resposta;
                        scanf(" %c", &resposta);
                        getchar();

                        if (resposta == 's' || resposta == 'S')
                        {
                            printf("\nCaminho pelos vertices:\n");
                            printf("=========================================\n");
                            printf("Caminho: ");

                            for (int i = 0; i < tamanho_caminho; i++)
                            {
                                printf("%d", caminho[i]);
                                if (i < tamanho_caminho - 1)
                                {
                                    printf(" -> ");
                                }
                            }
                            printf("\n");
                            printf("=========================================\n");
                        }
                    }
                    else
                    {
                        printf("Nao foi possivel encontrar uma solucao!\n");
                    }
                }
                else
                {
                    printf("Erro: Matriz ainda nao foi criada! Use a opcao 1 primeiro.\n");
                }
                break;
            }

            case 5:
            {
                printf("\nConfiguracoes do problema:\n");
                printf("\nConfiguracao inicial:\n");
                imprimir_configuracao(inicial);
                imprimir_estado_visual(inicial);

                printf("Configuracao final:\n");
                imprimir_configuracao(final);
                imprimir_estado_visual(final);
                break;
            }

            case 6:
            {
                printf("\nExecutando solucao completa...\n");

                // Mostrar configuracoes
                printf("\nConfiguracao inicial:\n");
                imprimir_configuracao(inicial);
                imprimir_estado_visual(inicial);

                printf("Configuracao final:\n");
                imprimir_configuracao(final);
                imprimir_estado_visual(final);

                // Criar matriz se ainda nao foi criada
                if (!matriz_criada)
                {
                    clock_t inicio_criacao = clock();
                    criar_matriz_adjacencia(matriz);
                    clock_t fim_criacao = clock();
                    double tempo_criacao = ((double)(fim_criacao - inicio_criacao)) / CLOCKS_PER_SEC;
                    printf("Tempo para criar matriz: %.6f segundos\n\n", tempo_criacao);
                    matriz_criada = 1;
                }

                // Encontrar caminho
                int indice_inicial = configuracao_para_indice(inicial);
                int indice_final = configuracao_para_indice(final);
                int caminho[NUM_CONFIGURACOES];

                printf("Executando algoritmo de Dijkstra...\n");
                clock_t inicio_dijkstra = clock();
                int tamanho_caminho = dijkstra(matriz, indice_inicial, indice_final, caminho);
                clock_t fim_dijkstra = clock();

                double tempo_dijkstra = ((double)(fim_dijkstra - inicio_dijkstra)) / CLOCKS_PER_SEC;

                if (tamanho_caminho > 0)
                {
                    printf("\nSolucao encontrada!\n");
                    printf("Numero minimo de movimentos: %d\n", tamanho_caminho - 1);
                    printf("Tempo do algoritmo de Dijkstra: %.6f segundos\n\n", tempo_dijkstra);

                    printf("Caminho pelos vertices:\n");
                    printf("=========================================\n");
                    printf("Caminho: ");

                    for (int i = 0; i < tamanho_caminho; i++)
                    {
                        printf("%d", caminho[i]);
                        if (i < tamanho_caminho - 1)
                        {
                            printf(" -> ");
                        }
                    }
                    printf("\n");

                    printf("=========================================\n");
                    printf("Solucao completa!\n");
                    printf("Numero total de movimentos: %d\n", tamanho_caminho - 1);

                    // Verificar se e o numero minimo teorico (2^n - 1)
                    int min_teorico = (1 << NUM_DISCOS) - 1; // 2^4 - 1 = 15
                    printf("Numero minimo teorico: %d movimentos\n", min_teorico);

                    if (tamanho_caminho - 1 == min_teorico)
                    {
                        printf("Solucao otima encontrada!\n");
                    }
                    else
                    {
                        printf("Solucao nao otima (diferenca: %d)\n", (tamanho_caminho - 1) - min_teorico);
                    }
                }
                else
                {
                    printf("Nao foi possivel encontrar uma solucao!\n");
                }
                break;
            }

            case 0:
                printf("Encerrando programa...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
            }

        } while (opcao != 0);

        free(matriz);
    }

    return resultado;
}