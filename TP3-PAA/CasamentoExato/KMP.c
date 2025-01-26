#include "KMP.h"

// Função auxiliar do KMP que computa a tabela de prefixos-sufixos
void computaTabela(char* padrao, int M, int* lps) {
    int tamanhoPref = 0;        // Tamanho do prefixo anterior
    lps[0] = 0;         // LPS significa Longest Prefix Suffix, é o vetor que armazena o tamanho do maior prefixo que também é sufixo
    int i = 1;          // Começa do segundo caractere
    
    // Constrói a tabela LPS
    while (i < M) { // Percorre o padrão
        if (padrao[i] == padrao[tamanhoPref]) {   // verifica se os caracteres batem
            tamanhoPref++;                        // Aumenta tamanho do prefixo
            lps[i] = tamanhoPref;                 // Armazena na tabela o tamanho do prefixo (o tamanho para cada letra do padrão)
            i++;                          // Avança para a próxima letra do padrão
        }
        else {                           // Se caracteres não batem
            if (tamanhoPref != 0) {              // Verifica se tem prefixo anterior
                tamanhoPref = lps[tamanhoPref - 1];      // Volta para prefixo anterior 
            }
            else {                       // Se não tem prefixo
                lps[i] = 0;              // Marca 0 na tabela (não tem prefixo)
                i++;                     // Avança para a próxima letra do padrão
            }
        }
    }
}

// Algoritmo KMP principal
void KMP(char* texto, char* padrao, int opcao) {
    clock_t inicio = clock();    // Marca tempo inicial
    int N = strlen(texto);       // Tamanho do texto
    int M = strlen(padrao);      // Tamanho do padrão
    int ocorrencias = 0;         // Contador de ocorrêcias
    
    // Aloca e computa tabela LPS
    // LPS significa Longest Prefix Suffix, é o vetor que armazena o tamanho do maior prefixo que também é sufixo
    int* lps = (int*)malloc(M * sizeof(int));
    computaTabela(padrao, M, lps);
    
    int i = 0;  // Índice para percorrer o texto
    int j = 0;  // Índice para percorrer o padrão
    
    // Loop principal do KMP
    while (i < N) {
        if (padrao[j] == texto[i]) {  // Se caracteres batem
            j++;                       // Avança em ambos
            i++;
        }
        
        if (j == M) {                 // Se encontrou padrão completo
            printf("Padrão encontrado na posição %d\n", i - j); // i é a posição final da substring, i-j é a posição inicial
            ocorrencias++;
            j = lps[j - 1];           // Volta para próxima possível ocorrência do padrão (é pra ajustar o padrão no texto sem precisar voltar tudo, ele vai pegar outra ocorrencia do padrão)
        }
        else if (i < N && padrao[j] != texto[i]) {  // Se caracteres não batem e não chegou ao fim do texto
            if (j != 0)               // Se não está no início do padrão
                j = lps[j - 1];       // Usa tabela LPS para voltar (ajusta o padrão no texto)
            else                      // Se está no início
                i++;                  // Avança no texto
        }
    }

    if (ocorrencias == 0) {
        printf("Padrão não encontrado no texto.\n");
    }
   
    // Calcula e mostra tempo gasto
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("------------------------------------\n");
    printf("Tempo KMP: %f segundos\n", tempo);
    printf("Total de ocorrências: %d\n", ocorrencias);
    if(opcao){
        // Abre o arquivo para escrita (append), cria um novo arquivo se não existir
        FILE *arquivo = fopen("Grafico/dadosKMP.txt", "a+");
        if (arquivo == NULL) {
            printf("\nErro ao abrir o arquivo para construcao do grafico!\n");
            return;
        }

        // Escreve o tamanho do texto e o tempo no arquivo
        fprintf(arquivo, "%d %f\n", N, tempo);
        // Fecha o arquivo
        fclose(arquivo);
    }   
    
    free(lps);  // Libera memória da tabela LPS
}