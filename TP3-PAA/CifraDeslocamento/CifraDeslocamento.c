#include "CifraDeslocamento.h"
#include <float.h>

// Tabela de frequências do português
const FrequenciaLetra FREQ_PT[tamanho_alfabeto] = { //
    {'a', 14.63}, {'b', 1.04}, {'c', 3.88}, {'d', 4.99}, {'e', 12.57},
    {'f', 1.02}, {'g', 1.30}, {'h', 1.28}, {'i', 6.18}, {'j', 0.40},
    {'k', 0.02}, {'l', 2.78}, {'m', 4.74}, {'n', 5.05}, {'o', 10.73},
    {'p', 2.52}, {'q', 1.20}, {'r', 6.53}, {'s', 7.81}, {'t', 4.34},
    {'u', 4.63}, {'v', 1.67}, {'w', 0.01}, {'x', 0.21}, {'y', 0.01},
    {'z', 0.47}
};

// Funções de criptografia e descriptografia
void criptografar(char *texto, int chave) {
    for (int i = 0; texto[i] != '\0'; i++) {
        if (isalpha(texto[i])) {
            texto[i] = tolower(texto[i]); // Transforma a letra em minuscula
            texto[i] = (texto[i] - 'a' + chave) % 26 + 'a'; // Desloca com base no 'a'
        }
    }
}

void descriptografar(char *texto, int chave) {
    criptografar(texto, 26 - (chave % 26));
}

// Função para calcular frequências
void calcular_frequencias(char *texto, FrequenciaLetra *freq) {
    int total = 0;
    int contagem[tamanho_alfabeto] = {0};
    
    // Inicializa array de frequências
    for (int i = 0; i < tamanho_alfabeto; i++) {
        freq[i].letra = 'a' + i;
        freq[i].frequencia = 0.0;
    }
    
    // Conta ocorrências
    for (int i = 0; texto[i] != '\0'; i++) {
        if (isalpha(texto[i])) {
            int idx = tolower(texto[i]) - 'a';
            contagem[idx]++;
            total++;
        }
    }
    
    // Calcula percentuais
    if (total > 0) {
        for (int i = 0; i < tamanho_alfabeto; i++) {
            freq[i].frequencia = (float)contagem[i] * 100 / total;
        }
    }
}

// Funcao para adivinhar a chave baseada nas frequências
int adivinhar_chave(FrequenciaLetra *freqTexto) {
    float menor_diferenca = 1000000.0f; // Valor inicial alto para comparação
    int chave_encontrada = 0;
    
    // Testa cada possível chave (0 a 25)
    for (int chave = 0; chave < 26; chave++) {
        float diferenca_total = 0.0f;
        
        // Compara a frequência de cada letra com a tabela de português
        for (int i = 0; i < 26; i++) {
            // Calcula qual letra seria após o deslocamento
            int posicao_deslocada = (i + chave) % 26;
            
            // Calcula a diferença entre a frequência encontrada e a esperada
            float diferenca = freqTexto[i].frequencia - FREQ_PT[posicao_deslocada].frequencia;
            diferenca_total += diferenca * diferenca; // Diferença ao quadrado
        }
        
        // Se encontrou uma diferença menor, atualiza a chave
        if (diferenca_total < menor_diferenca) {
            menor_diferenca = diferenca_total;
            chave_encontrada = chave;
        }
    }
    
    // Retorna a chave invertida para obter a chave original usada na criptografia
    return (26 - chave_encontrada) % 26;
}

// Função para ler arquivo
/*int ler_arquivo(const char *nome_arquivo, char *texto) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) {
        return 0;
    }
    
    size_t lido = fread(texto, 1, tamanho_maximo - 1, fp);
    texto[lido] = '\0';
    fclose(fp);
    return 1;
}

// Função para salvar arquivo
int salvar_arquivo(const char *nome_arquivo, const char *texto) {
    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp) {
        return 0;
    }
    
    fprintf(fp, "%s", texto);
    fclose(fp);
    return 1;
}*/

int ler_arquivo(const char *nome_arquivo, char *texto) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) {
        return 0;
    }
    
    // Limpa o buffer antes de usar
    memset(texto, 0, tamanho_maximo);
    
    char linha[1024];  // Buffer temporário para cada linha
    size_t pos = 0;    // Posição atual no buffer texto
    
    // Lê linha por linha
    while (fgets(linha, sizeof(linha), fp) != NULL) {
        size_t len = strlen(linha);
        
        // Verifica se ainda há espaço no buffer
        if (pos + len >= tamanho_maximo - 1) {
            break;  // Buffer cheio
        }
        
        // Copia a linha para o buffer principal
        strcpy(texto + pos, linha);
        pos += len;
    }
    
    fclose(fp);
    return 1;
}

int salvar_arquivo(const char *nome_arquivo, const char *texto) {
    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp) {
        return 0;
    }
    
    // Escreve o texto completo
    fputs(texto, fp);
    
    fclose(fp);
    return 1;
}

int main() {
    char texto[tamanho_maximo];
    char nome_arquivo[100];
    char nome_saida[120];
    int opcao, chave;
    FrequenciaLetra frequencias[tamanho_alfabeto];
    
    srand(time(NULL));  // Inicializa gerador de números aleatórios
    
    while (1) {
        printf("\n=== Menu ===\n");
        printf("1. Criptografar arquivo\n");
        printf("2. Descriptografar arquivo\n");
        printf("3. Usar chave aleatoria\n");
        printf("4. Sair\n");
        
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();  // Limpa o buffer
        
        if (opcao == 4) break;
        
        printf("Digite o nome do arquivo de entrada: ");
        fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
        nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;  // Remove \n
        
        if (!ler_arquivo(nome_arquivo, texto)) {
            printf("Erro: Arquivo nao encontrado!\n");
            continue;
        }
        
        if (opcao == 1 || opcao == 2) {
            printf("Digite a chave (numero inteiro): ");
            scanf("%d", &chave);
            chave = ((chave % 26) + 26) % 26;  // Normaliza a chave
        } else if (opcao == 3) {
            chave = rand() % 25 + 1;  // 1 a 25
            printf("\nChave aleatoria gerada: %d\n", chave);
        }
        
        if (opcao == 1 || opcao == 3) {
            criptografar(texto, chave);
            if (!salvar_arquivo("texto_criptografado.txt", texto)) {
                printf("Erro ao salvar arquivo criptografado!\n");
                continue;
            }
            printf("\nResultado salvo em: texto_criptografado.txt\n");
        } else if (opcao == 2) {
            descriptografar(texto, chave);
            if (!salvar_arquivo("texto_descriptografado.txt", texto)) {
                printf("Erro ao salvar arquivo descriptografado!\n");
                continue;
            }
            printf("\nResultado salvo em: texto_descriptografado.txt\n");
        }
        
        printf("\nConteudo do arquivo:\n%s\n", texto);


        
        // Se usou chave aleatória, calcula e mostra frequências
        if (opcao == 3) {
            calcular_frequencias(texto, frequencias);
            
            printf("\nFrequencias encontradas:\n");
            printf("Letra | Frequencia\n");
            printf("--------------------\n");
            for (int i = 0; i < tamanho_alfabeto; i++) {
                printf("%c     | %.2f%%\n", 
                       frequencias[i].letra, 
                       frequencias[i].frequencia);
            }
            
            int chave_estimada = adivinhar_chave(frequencias);
            printf("\nChave estimada: %d\n", chave_estimada);
            printf("Chave real: %d\n", chave);
        }
    }
    
    return 0;
}