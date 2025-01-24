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

void criptografar(char *texto, int chave);
void descriptografar(char *texto, int chave);
void calcular_frequencias(char *texto, FrequenciaLetra *freq);
int adivinhar_chave(FrequenciaLetra *freqTexto);
int ler_arquivo(const char *nome_arquivo, char *texto);
int salvar_arquivo(const char *nome_arquivo, const char *texto);

