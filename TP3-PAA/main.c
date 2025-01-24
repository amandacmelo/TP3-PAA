
#include <stdio.h>    // Para funções de entrada/saída
#include <string.h>   // Para manipulação de strings (strlen)
#include <stdlib.h>   // Para alocação dinâmica (malloc, free)
#include <time.h>     // Para medir tempo de execução
#include <ctype.h>     // Funções para manipulação de caracteres (ispunct, tolower)
#include "CasamentoExato/ForcaBruta.h"
#include "CasamentoExato/KMP.h"


void removeAcentosPontuacao(char *str) {
    // Mapeamento de caracteres acentuados para seus equivalentes sem acento
    char *substituicoes[][2] = {
        {"á", "a"}, {"à", "a"}, {"ã", "a"}, {"â", "a"}, {"ä", "a"},
        {"é", "e"}, {"è", "e"}, {"ê", "e"}, {"ë", "e"},
        {"í", "i"}, {"ì", "i"}, {"î", "i"}, {"ï", "i"},
        {"ó", "o"}, {"ò", "o"}, {"õ", "o"}, {"ô", "o"}, {"ö", "o"},
        {"ú", "u"}, {"ù", "u"}, {"û", "u"}, {"ü", "u"},
        {"ý", "y"}, {"ñ", "n"}, {"ç", "c"},
        {"Á", "a"}, {"À", "a"}, {"Ã", "a"}, {"Â", "a"}, {"Ä", "a"},
        {"É", "e"}, {"È", "e"}, {"Ê", "e"}, {"Ë", "e"},
        {"Í", "i"}, {"Ì", "i"}, {"Î", "i"}, {"Ï", "i"},
        {"Ó", "o"}, {"Ò", "o"}, {"Õ", "o"}, {"Ô", "o"}, {"Ö", "o"},
        {"Ú", "u"}, {"Ù", "u"}, {"Û", "u"}, {"Ü", "u"},
    };
    
    // Cria um array para armazenar o resultado processado
    char resultado[strlen(str) + 1];
    resultado[0] = '\0';  // Inicializa como string vazia
    
    // Ponteiro para navegar pela string original
    char *pos = str;
    
    // Percorre cada caractere da string original
    while (*pos) {
        // Flag para verificar se o caractere foi substituído
        int substituido = 0;
        
        // Remove pontuação: ignora caracteres de pontuação
        if (ispunct(*pos)) {
            pos++;
            continue;
        }
        
        // Verifica se o caractere atual é um caractere acentuado
        for (int i = 0; i < sizeof(substituicoes) / sizeof(substituicoes[0]); i++) {
            // Compara o caractere atual com os padrões de acentuação
            if (strncmp(pos, substituicoes[i][0], strlen(substituicoes[i][0])) == 0) {
                // Concatena o equivalente sem acento no resultado
                strcat(resultado, substituicoes[i][1]);
                // Avança o ponteiro pelo tamanho do caractere acentuado
                pos += strlen(substituicoes[i][0]);
                substituido = 1;
                break;
            }
        }
        
        // Se não foi um caractere acentuado, adiciona em minúsculo
        if (!substituido) {
            // Converte para minúsculo e adiciona ao resultado
            char temp[2] = {tolower(*pos), '\0'};
            strcat(resultado, temp);
            pos++;
        }
    }
    
    // Copia o resultado processado de volta para a string original
    strcpy(str, resultado);
}


// Função para ler arquivo de texto
char* carregaTexto(const char* nomeArquivo) {
    // Tenta abrir o arquivo
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return NULL;
    }
    
    // Determina tamanho do arquivo
    fseek(arquivo, 0, SEEK_END);     // Vai para o fim do arquivo
    long tamanho = ftell(arquivo);    // Pega posição atual, fim do arquivo(tamanho)
    rewind(arquivo);                  // Volta para o início
    
    // Aloca memória para o texto
    char* texto = (char*)malloc(tamanho + 1); // +1 para o terminador de string e evitar erros
    if (texto == NULL) {
        printf("Erro de alocação de memória\n");
        fclose(arquivo);
        return NULL;
    }
    
    // Lê o conteúdo do arquivo, passa para texto e salva o tamanho lido em resultado
    size_t resultado = fread(texto, 1, tamanho, arquivo); 
    if (resultado != tamanho) { // Verifica se leu tudo que era esperado
        printf("Erro ao ler o arquivo\n");
        free(texto);
        fclose(arquivo);
        return NULL;
    }
    
    texto[tamanho] = '\0';  // Adiciona terminador de string
    fclose(arquivo);        // Fecha arquivo
    return texto;
}



// Função principal
int main() {
    char nomeArquivo[100]; 
    char padrao[100];      
    
    // Lê nome do arquivo
    printf("Digite o nome do arquivo de texto: ");
    scanf("%s", nomeArquivo);
    
    // Lê padrão a ser buscado
    printf("Digite o padrão a ser buscado: ");
    scanf(" %[^\n]s", padrao);   // Lê linha inteira incluindo espaços

    // Carrega texto do arquivo
    char* texto = carregaTexto(nomeArquivo);
    if (texto == NULL) {
        return 1;  // Retorna com erro se falhou
    }
    removeAcentosPontuacao(texto);  // Remove acentos e pontuação do texto
    removeAcentosPontuacao(padrao); // Remove acentos e pontuação do padrão
 
    // Executa os dois algoritmos
    printf("\n=== Busca usando Força Bruta ===\n");
    forcaBruta(texto, padrao);
    
    printf("\n=== Busca usando KMP ===\n");
    KMP(texto, padrao);
    
    free(texto);  // Libera memória alocada
    return 0;
}