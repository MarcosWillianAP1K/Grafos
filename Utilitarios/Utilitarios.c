#include <stdio.h>
#include <stdlib.h>
#include "Utilitarios.h"


void verificar_alocacao(void *ptr, const char *mensagem)
{
    if (!ptr)
    {
        fprintf(stderr, "Erro de alocacao: %s\n", mensagem);
        exit(EXIT_FAILURE);
    }
}