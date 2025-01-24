import matplotlib.pyplot as plt

# Listas para armazenar os dados dos dois algoritmos
tamanhos = []
tempos_KMP = []
tempos_ForcaBruta = []

# Ler os dados dos dois arquivos
try:
    # Lendo dados do primeiro algoritmo
    with open("Grafico/dadosKMP.txt", "r") as file:
        for line in file:
            tamanho, tempo = line.split()
            tamanhos.append(int(tamanho))
            tempos_KMP.append(float(tempo))
            
    # Lendo dados do segundo algoritmo
    with open("Grafico/dadosForcaBruta.txt", "r") as file:
        tempos_ForcaBruta_temp = []
        for line in file:
            tamanho, tempo = line.split()
            tempos_ForcaBruta_temp.append(float(tempo))
        tempos_ForcaBruta = tempos_ForcaBruta_temp  # Garantir que os tempos correspondam aos mesmos tamanhos (caso um arquivo tenha mais linhas que o outro)

except ValueError:
    print("Erro ao ler os dados dos arquivos. Verifique se os arquivos estão no formato correto.")
except FileNotFoundError:
    print("Erro: Arquivo não encontrado. Verifique se o caminho está correto.")

# Criar o gráfico
plt.figure(figsize=(10, 6)) #define o tamanho do gráfico

# Plotar as duas linhas
plt.plot(tamanhos, tempos_KMP, marker='o', label='KMP', color='blue')
plt.plot(tamanhos, tempos_ForcaBruta, marker='s', label='Força Bruta', color='red')

# Personalizar o gráfico
plt.xlabel('Tamanho do Texto')
plt.ylabel('Tempo de Execução (segundos)')
plt.title('Comparação de Desempenho: Força Bruta vs KMP')
plt.grid(True)
plt.legend()  # Adiciona a legenda

# Mostrar o gráfico
plt.show()