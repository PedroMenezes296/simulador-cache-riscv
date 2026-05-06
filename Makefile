# Nome do executável
EXEC = simulador_cache

# Arquivos fonte
SRC = src/main.c src/algoritmos/lru.c src/algoritmos/mockingjay.c

# Flags de compilação (mostra todos os avisos)
CFLAGS = -Wall -Iinclude

all:
	gcc $(CFLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC)