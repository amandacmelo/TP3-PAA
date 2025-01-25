#all: run

run: compile
	./run $(ARGS)

compile: CasamentoExato/KMP.o CasamentoExato/ForcaBruta.o CifraDeslocamento/CifraDeslocamento.o main.o
	gcc -o run CasamentoExato/KMP.o CasamentoExato/ForcaBruta.o CifraDeslocamento/CifraDeslocamento.o main.o

CasamentoExato/KMP.o: CasamentoExato/KMP.c
	gcc -c CasamentoExato/KMP.c -o CasamentoExato/KMP.o

CasamentoExato/ForcaBruta.o: CasamentoExato/ForcaBruta.c
	gcc -c CasamentoExato/ForcaBruta.c -o CasamentoExato/ForcaBruta.o

CifraDeslocamento/CifraDeslocamento.o: CifraDeslocamento/CifraDeslocamento.c
	gcc -c CifraDeslocamento/CifraDeslocamento.c -o CifraDeslocamento/CifraDeslocamento.o

main.o: main.c
	gcc -c main.c -o main.o

# Limpar arquivos compilados
clean:
	rm -f run CasamentoExato/*.o main.o

.PHONY: compile clean