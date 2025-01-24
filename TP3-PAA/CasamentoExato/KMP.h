#ifndef KMP_H
#define KMP_H

// Inclusão das bibliotecas necessárias
#include <stdio.h>    // Para funções de entrada/saída
#include <string.h>   // Para manipulação de strings (strlen)
#include <stdlib.h>   // Para alocação dinâmica (malloc, free)
#include <time.h>     // Para medir tempo de execução

void computaLPS(char* padrao, int M, int* lps);
void KMP(char* texto, char* padrao);

#endif 