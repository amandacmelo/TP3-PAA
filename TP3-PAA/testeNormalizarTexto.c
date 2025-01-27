#include <stdio.h>     // Biblioteca padrão de entrada/saída
#include <string.h>    // Funções para manipulação de strings
#include <ctype.h>     // Funções para manipulação de caracteres (ispunct, tolower)

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
        {"Ý", "y"}, {"Ñ", "n"}, {"Ç", "c"}
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

int main() {
    // Array para armazenar o texto de entrada
    char texto[100];
    
    // Solicita entrada do usuário
    printf("Digite o texto: ");
    
    // Lê a linha de entrada, incluindo espaços
    fgets(texto, sizeof(texto), stdin);
    
    // Remove a quebra de linha do final da string
    texto[strcspn(texto, "\n")] = 0;
    
    // Chama a função para processar o texto
    removeAcentosPontuacao(texto);
    
    // Imprime o texto processado
    printf("Texto processado: %s\n", texto);
    
    return 0;
}