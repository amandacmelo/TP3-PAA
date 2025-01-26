#include <stdio.h>    // Para funções de entrada/saída
#include <string.h>   // Para manipulação de strings (strlen)
#include <stdlib.h>   // Para alocação dinâmica (malloc, free)
#include <time.h>     // Para medir tempo de execução
#include <unistd.h>   // Para função sleep
#include <ctype.h>    // Funções para manipulação de caracteres (ispunct, tolower)
#include <dirent.h> 
#include "CasamentoExato/ForcaBruta.h"
#include "CasamentoExato/KMP.h"
#include "CifraDeslocamento/CifraDeslocamento.h"

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
        sleep(1.5);
        system("clear");
        fclose(arquivo);
        return NULL;
    }
    
    // Lê o conteúdo do arquivo, passa para texto e salva o tamanho lido em resultado
    size_t resultado = fread(texto, 1, tamanho, arquivo); 
    if (resultado != tamanho) { // Verifica se leu tudo que era esperado
        printf("Erro ao ler o arquivo\n");
        sleep(1.5);
        system("clear");
        free(texto);
        fclose(arquivo);
        return NULL;
    }
    
    texto[tamanho] = '\0';  // Adiciona terminador de string
    fclose(arquivo);        // Fecha arquivo
    return texto;
}

// Função para execultar o casamento de cada arquivo de um diretório
void preencheGrafico(char *diretorio, char *padrao) {
    DIR *d;  // Ponteiro para o diretório
    struct dirent *dir;  // Estrutura para armazenar informações dos arquivos

    d = opendir(diretorio);  // Abre o diretório
    if (d) {  // Verifica se o diretório foi aberto com sucesso
        while ((dir = readdir(d)) != NULL) {  // Lê cada entrada do diretório
                
            // Pule diretórios e entradas vazias
            if (strcmp(dir->d_name, ".") == 0 || 
                strcmp(dir->d_name, "..") == 0 || 
                dir->d_name[0] == '\0') {
                continue;
            }

            // Constrói o caminho completo do arquivo
            char caminhoArquivo[1024];
            snprintf(caminhoArquivo, sizeof(caminhoArquivo), "%s/%s", diretorio, dir->d_name);
            printf("Arquivo: %s\n", dir->d_name);
            
            // Carrega texto do arquivo
            char* texto = carregaTexto(caminhoArquivo);
            if (texto == NULL) {
                printf("Erro: Arquivo nao encontrado!\n");
                sleep(1.5);
                system("clear");
                closedir(d);
                continue;
            }
            removeAcentosPontuacao(texto);  // Remove acentos e pontuação do texto

            // Executa os dois algoritmos
            printf("\n===== Busca usando Força Bruta =====\n");
            forcaBruta(texto, padrao, 1);
            printf("====================================\n");
            printf("\n========= Busca usando KMP =========\n");
            KMP(texto, padrao, 1);
            printf("===================================\n");
            
              
            free(texto);  // Libera a memória alocada para o texto
            printf("Pressione Enter para continuar... \n");
            getchar();
            getchar();
            system("clear");
        }
        closedir(d);  // Fecha o diretório
    }
}

// Função principal
int main() {  
    int opcao;
    srand(time(NULL));  // Inicializa gerador de números aleatórios
    
    while (1){
        printf("\n--------------- MENU PRINCIPAL -------------\n");
        printf("1. Casamento de padrões\n");
        printf("2. Cifra de deslocamento\n");
        printf("3. Sair\n");
        printf("--------------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer
        printf("--------------------------------------------\n");
    
        if (opcao < 1 || opcao > 3) {
            printf("\nOpção inválida!\n");
            sleep(1);
            system("clear");
            continue;
        }
        system("clear"); 
       
        switch (opcao){
        case 1:{
            int opcao_grafico;
            char nomeArquivo[100]; 
            char padrao[100];  
            printf("\n----------- CASAMENTO DE PADROES -----------\n");
            printf("1. Carregar arquivo de texto\n");
            printf("2. Carregar diretorio para gerar dados para o gráfico\n");
            scanf("%d", &opcao_grafico);
            if(opcao_grafico == 1){
                printf("Digite o nome do arquivo de texto: ");
                fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
                nomeArquivo[strcspn(nomeArquivo, "\n")] = 0;  // Remove newline

                // Verifica se o nome do arquivo está vazio
                if (strcmp(nomeArquivo, "") == 0) {
                    printf("Por favor, carregue antes um arquivo de dados! \n");
                    printf("Pressione qualquer tecla para continuar... \n");
                    getchar(); 
                    system("clear");
                    continue;
                }
                        
                // Lê padrão a ser buscado
                printf("Digite o padrão a ser buscado: ");
                scanf(" %[^\n]s", padrao);   // Lê linha inteira incluindo espaços
                printf("--------------------------------------------\n");

                // Verifica se o nome do arquivo está vazio
            if (strcmp(nomeArquivo, "") == 0) {
                    printf("Por favor, carregue antes um arquivo de dados! \n");
                    printf("Pressione qualquer tecla para continuar... \n");
                    getchar(); 
                    system("clear");
                    continue;
                }   
                // Carrega texto do arquivo
                char* texto = carregaTexto(nomeArquivo);
                if (texto == NULL) {
                    printf("Erro: Arquivo nao encontrado!\n");
                    sleep(1.5);
                    system("clear");
                    continue;
                }

                removeAcentosPontuacao(texto);  // Remove acentos e pontuação do texto
                removeAcentosPontuacao(padrao); // Remove acentos e pontuação do padrão
            
                
                // Executa os dois algoritmos
                printf("\n===== Busca usando Força Bruta =====\n");
                forcaBruta(texto, padrao, 0);
                printf("====================================\n");
                printf("\n========= Busca usando KMP =========\n");
                KMP(texto, padrao, 0);
                printf("===================================\n");
                free(texto);
                printf("Pressione Enter para continuar... \n");
                getchar();
                getchar();
                system("clear");
            } else if(opcao_grafico == 2){
                char diretorio[100];
                printf("Digite o nome do diretorio: ");
                scanf(" %[^\n]s", diretorio);   // Lê linha inteira incluindo espaços
                diretorio[strcspn(diretorio, "\n")] = 0;  // Remove newline

                // Lê padrão a ser buscado
                printf("Digite o padrão a ser buscado: ");
                scanf(" %[^\n]s", padrao);   // Lê linha inteira incluindo espaços
                printf("--------------------------------------------\n");

                // Verifica se o nome do diretório está vazio
                if (strcmp(diretorio, "") == 0) {
                    printf("Por favor, carregue antes um diretório de dados! \n");
                    printf("Pressione qualquer tecla para continuar... \n");
                    getchar(); 
                    system("clear");
                    continue;
                }

                removeAcentosPontuacao(padrao); // Remove acentos e pontuação do padrão
                preencheGrafico(diretorio, padrao);
                printf("Diretório execultado com sucesso! \n");
                printf("Pressione Enter para continuar... \n");
                getchar();
                getchar();
                system("clear");
            }
            break;
        }
        case 2:
        {
            char texto_cifra[tamanho_maximo];
            char nome_arquivo[100];
            int subopcao, chave;
            FrequenciaLetra frequencias[tamanho_alfabeto];
            
            while (1) {
                printf("\n---------- CIFRA DE DESLOCAMENTO -----------\n");
                printf("1. Criptografar arquivo\n");
                printf("2. Descriptografar arquivo\n");
                printf("3. Usar chave aleatoria\n");
                printf("4. Voltar ao menu principal\n");
                printf("--------------------------------------------\n");
                
                printf("\nEscolha uma opcao: ");
                scanf("%d", &subopcao);
                getchar();  // Limpa o buffer

                if (subopcao < 1 || subopcao > 4) {
                    printf("\nOpção inválida!\n");
                    sleep(1);
                    system("clear");
                    continue;
                }
                
                if (subopcao == 4) {
                    system("clear");
                    break;  // Sai do submenu e volta ao menu principal
                }
                
                printf("Digite o nome do arquivo de entrada: ");
                fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
                nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;  // Remove \n
                
                if (strcmp(nome_arquivo, "") == 0) {
                    printf("Por favor, carregue antes um arquivo de dados! \n");
                    printf("Pressione qualquer tecla para continuar... \n");
                    getchar(); 
                    system("clear");
                    continue;
                }    
                
                if (!ler_arquivo(nome_arquivo, texto_cifra)) {
                    printf("Erro: Arquivo nao encontrado!\n");
                    sleep(1);
                    system("clear");
                    continue;
                }

                removeAcentosPontuacao(texto_cifra);  // Remove acentos e pontuação do texto
                
                if (subopcao == 1 || subopcao == 2) {
                    printf("Digite a chave (numero inteiro): ");
                    scanf("%d", &chave);
                    getchar();  // Limpa o buffer
                    chave = ((chave % 26) + 26) % 26;  // Normaliza a chave
                } else if (subopcao == 3) {
                    chave = rand() % 25 + 1;  // 1 a 25
                    printf("\nChave aleatoria gerada: %d\n", chave);
                }
                
                if (subopcao == 1 || subopcao == 3) {
                    criptografar(texto_cifra, chave);
                    if (!salvar_arquivo("texto_criptografado.txt", texto_cifra)) {
                        printf("Erro ao salvar arquivo criptografado!\n");
                        continue;
                    }
                    printf("--------------------------------------------\n");
                    printf("\nResultado salvo em: texto_criptografado.txt\n");
                } else if (subopcao == 2) {
                    descriptografar(texto_cifra, chave);
                    if (!salvar_arquivo("texto_descriptografado.txt", texto_cifra)) {
                        printf("Erro ao salvar arquivo descriptografado!\n");
                        continue;
                    }
                    printf("--------------------------------------------\n");
                    printf("\nResultado salvo em: texto_descriptografado.txt\n");
                }
                
                printf("\nConteudo do arquivo:\n%s\n", texto_cifra);
                // Se usou chave aleatória, calcula e mostra frequências
                if (subopcao == 3) {
                    calcular_frequencias(texto_cifra, frequencias);
                    
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
                printf("\nPressione Enter para continuar... \n");
                getchar();  // Aguarda o Enter antes de continuar
                system("clear");
            }
            break;
        }
        case 3:
            printf("Saindo.... \n");
            sleep(1);
            printf("FIM.\n");
            exit(0);
        }
    }
    
    return 0;
}
