#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h> // Biblioteca necessária para ler diretórios

#include "../include/cache.h"
#include "../include/lru.h"
#include "../include/mockingjay.h"

#define MAX_TRACES 50
#define MAX_FILENAME_LEN 256

int main() {
    char algoritmos[2][15] = {"LRU", "Mockingjay"};
    char traces[MAX_TRACES][MAX_FILENAME_LEN];
    int num_traces = 0;

    // Loop principal infinito até o usuário escolher sair (opção 0)
    while (1) {
        printf("\n==========================================\n");
        printf("   SIMULADOR DE CACHE - MENU INTERATIVO\n");
        printf("==========================================\n");

        // 1. Escolher o Algoritmo
        printf("\nEscolha o Algoritmo:\n");
        printf("1. LRU\n");
        printf("2. Mockingjay\n");
        printf("0. Sair\n");
        printf("Opcao: ");

        int op_algo;
        if (scanf("%d", &op_algo) != 1 || op_algo == 0) {
            printf("\nEncerrando o simulador...\n");
            break;
        }
        if (op_algo < 1 || op_algo > 2) {
            printf("\n[!] Opcao invalida. Tente novamente.\n");
            continue;
        }

        char *algoritmo_escolhido = algoritmos[op_algo - 1];

        // 2. Ler a pasta e listar os Traces
        DIR *d;
        struct dirent *dir;
        d = opendir("traces");
        num_traces = 0;

        if (d) {
            printf("\nTraces disponiveis na pasta 'traces/':\n");
            while ((dir = readdir(d)) != NULL) {
                // Ignora os ponteiros de pasta oculta do sistema "." e ".."
                if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                    strcpy(traces[num_traces], dir->d_name);
                    printf("%d. %s\n", num_traces + 1, traces[num_traces]);
                    num_traces++;
                    if (num_traces >= MAX_TRACES) break; // Limite de segurança
                }
            }
            closedir(d);
        } else {
            printf("\n[!] Erro: Nao foi possivel abrir a pasta 'traces'. Verifique se ela existe na raiz do projeto.\n");
            continue;
        }

        if (num_traces == 0) {
            printf("\n[!] Nenhum arquivo encontrado na pasta 'traces/'. Adicione alguns traces para testar.\n");
            continue;
        }

        // 3. Escolher o Trace
        printf("0. Voltar ao menu anterior\n");
        printf("Escolha o Trace que deseja rodar: ");
        int op_trace;
        if (scanf("%d", &op_trace) != 1 || op_trace == 0) {
            continue; // Volta para o início do loop (menu de algoritmos)
        }
        if (op_trace < 1 || op_trace > num_traces) {
            printf("\n[!] Opcao invalida.\n");
            continue;
        }

        // Montar o caminho relativo completo do arquivo (ex: "traces/trace_simples.txt")
        char caminho_arquivo[MAX_FILENAME_LEN + 10];
        sprintf(caminho_arquivo, "traces/%s", traces[op_trace - 1]);

        // 4. Abrir arquivo e executar simulação
        FILE *file = fopen(caminho_arquivo, "r");
        if (!file) {
            printf("\n[!] Erro ao abrir o arquivo: %s\n", caminho_arquivo);
            continue;
        }

        CacheStats stats = {0, 0, 0};
        int is_lru = (op_algo == 1);
        int is_mockingjay = (op_algo == 2);

        if (is_lru) inicializar_cache_lru();
        else if (is_mockingjay) inicializar_cache_mockingjay();

        uint32_t endereco;
        printf("\n--- Processando %s com politica %s ---\n", traces[op_trace - 1], algoritmo_escolhido);
        
        while (fscanf(file, "%x", &endereco) != EOF) {
            stats.acessos_totais++;
            int hit = 0;

            if (is_lru) hit = acessar_cache_lru(endereco);
            else if (is_mockingjay) hit = acessar_cache_mockingjay(endereco);

            if (hit) stats.hits++;
            else stats.misses++;
            
            // Para debugar passo a passo, basta descomentar a linha abaixo:
            // printf("Acesso %d: Endereco 0x%X -> %s\n", stats.acessos_totais, endereco, hit ? "HIT" : "MISS");
        }

        fclose(file);

        // 5. Exibindo os Resultados
        double hit_rate = (stats.acessos_totais > 0) ? ((double)stats.hits / stats.acessos_totais) * 100.0 : 0.0;
        
        printf("\n=== RESULTADOS: %s ===\n", algoritmo_escolhido);
        printf("Arquivo: %s\n", traces[op_trace - 1]);
        printf("Acessos Totais: %d\n", stats.acessos_totais);
        printf("Hits: %d\n", stats.hits);
        printf("Misses: %d\n", stats.misses);
        printf("Hit Rate: %.2f%%\n", hit_rate);

        // Pausa o programa para você ler o resultado antes de limpar/reimprimir o menu
        printf("\nPressione ENTER para continuar...");
        getchar(); // Limpa o buffer do \n deixado pelo scanf anterior
        getchar(); // Aguarda o usuário apertar Enter de fato
    }

    return 0;
}