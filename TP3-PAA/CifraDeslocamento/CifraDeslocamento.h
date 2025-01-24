#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define tamanho_maximo 10000
#define tamanho_alfabeto 26

// Estrutura para armazenar frequências
typedef struct {
    char letra;
    float frequencia;
} FrequenciaLetra;