#include "ForcaBruta.h"
// Algoritmo de Força Bruta
void forcaBruta(char* texto, char* padrao, int opcao) {

    clock_t inicio = clock();    // Marca o tempo inicial
    int N = strlen(texto);       // Obtém tamanho do texto
    int M = strlen(padrao);      // Obtém tamanho do padrão
    int ocorrencias = 0;         // Contador de ocorrências encontradas
    
    // Loop principal - verifica cada posição possível no texto
    for (int i = 0; i <= N - M; i++) { // Até N - M para não passar do final (garante espaço para o padrão)
        int j; 
        // Loop interno - compara o padrão com a substring atual
        for (j = 0; j < M; j++) {
            // Se encontrar um caractere diferente, interrompe
            if (texto[i + j] != padrao[j]) // texto[i+j] pq i é a posição inicial da substring e j anda sobre ela  
                break;
        }

        // Se j chegou ao fim do padrão, encontramos uma ocorrência
        if (j == M) {
            printf("Padrão encontrado na posição %d\n", i);
            ocorrencias++;
        }
       
    }
    // Se não encontrou nenhuma ocorrência
    if (ocorrencias == 0) {
        printf("Padrão não encontrado no texto.\n");
    }
    
    // Calcula e mostra o tempo gasto
    clock_t fim = clock(); 
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("------------------------------------\n");
    printf("Tempo Força Bruta: %f segundos\n", tempo);
    printf("Total de ocorrências: %d\n", ocorrencias);
    // Se opcao == 1, salva os dados para construção do gráfico
    if (opcao == 1) {
        // Abre o arquivo para escrita (append), cria um novo arquivo se não existir
        FILE *arquivo = fopen("Grafico/dadosForcaBruta.txt", "a+");
        if (arquivo == NULL) {
            printf("\nErro ao abrir o arquivo para construcao do grafico!\n");
            return;
        }

        // Escreve o tamanho do texto e o tempo no arquivo
        fprintf(arquivo, "%d %f\n", N, tempo);

        // Fecha o arquivo
        fclose(arquivo);
    }
}

