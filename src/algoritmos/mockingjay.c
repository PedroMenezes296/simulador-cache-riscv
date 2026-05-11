#include "../../include/mockingjay.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_HISTORICO 10

typedef struct {
    uint32_t tag;
    int valid;
} CacheLine;

CacheLine cache[NUM_CONJUNTOS][NUM_VIAS];

uint32_t *trace = NULL;
int trace_size = 0;
int current_index = 0;

typedef struct {
    int execucao;
    int hits;
    int misses;
    double hit_rate;
} Historico;

Historico historico[MAX_HISTORICO];
int num_historico = 0;
int execucao_atual = 1;

void set_trace_mockingjay(uint32_t *t, int size) {
    trace = t;
    trace_size = size;
}

void inicializar_cache_mockingjay() {
    for (int i = 0; i < NUM_CONJUNTOS; i++) {
        for (int j = 0; j < NUM_VIAS; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
        }
    }
    current_index = 0;
    printf("[Mockingjay] Cache inicializada.\n");
}

int calcular_conjunto(uint32_t endereco) {
    return (endereco / BLOCK_SIZE_BYTES) % NUM_CONJUNTOS;
}

uint32_t calcular_tag(uint32_t endereco) {
    return endereco / (BLOCK_SIZE_BYTES * NUM_CONJUNTOS);
}

int acessar_cache_mockingjay(uint32_t endereco) {
    int conjunto = calcular_conjunto(endereco);
    uint32_t tag = calcular_tag(endereco);

    // Verificar hit
    for (int v = 0; v < NUM_VIAS; v++) {
        if (cache[conjunto][v].valid && cache[conjunto][v].tag == tag) {
            return 1; // Hit
        }
    }

    // Miss
    int via_substituir = -1;
    if (!cache[conjunto][0].valid) {
        via_substituir = 0;
    } else if (!cache[conjunto][1].valid) {
        via_substituir = 1;
    } else {
        // Cache cheia, usar Belady
        int distancias[2];
        for (int v = 0; v < NUM_VIAS; v++) {
            uint32_t tag_atual = cache[conjunto][v].tag;
            distancias[v] = INT_MAX;
            for (int k = current_index + 1; k < trace_size; k++) {
                uint32_t end_futuro = trace[k];
                uint32_t tag_futuro = calcular_tag(end_futuro);
                if (tag_futuro == tag_atual && calcular_conjunto(end_futuro) == conjunto) {
                    distancias[v] = k - current_index;
                    break;
                }
            }
        }
        if (distancias[0] >= distancias[1]) {
            via_substituir = 0;
        } else {
            via_substituir = 1;
        }
        printf("Decisão: Bloco %c (distância %d) substituído por %c (distância %d)\n", 'A' + cache[conjunto][via_substituir].tag, distancias[via_substituir], 'A' + tag, distancias[1 - via_substituir]);
    }

    // Substituir
    cache[conjunto][via_substituir].tag = tag;
    cache[conjunto][via_substituir].valid = 1;
    current_index++;
    return 0; // Miss
}

void imprimir_estado_mockingjay(uint32_t endereco) {
    int conjunto = calcular_conjunto(endereco);
    printf("| Cache: ");
    for (int v = 0; v < NUM_VIAS; v++) {
        if (cache[conjunto][v].valid) {
            printf("%c", 'A' + cache[conjunto][v].tag);
        } else {
            printf("-");
        }
        if (v < NUM_VIAS - 1) printf(", ");
    }
    printf("\n");
}

void finalizar_execucao_mockingjay(int hits, int misses) {
    int acessos = hits + misses;
    double hit_rate = (acessos > 0) ? ((double)hits / acessos) * 100.0 : 0.0;

    // Adicionar ao histórico
    if (num_historico < MAX_HISTORICO) {
        historico[num_historico].execucao = execucao_atual;
        historico[num_historico].hits = hits;
        historico[num_historico].misses = misses;
        historico[num_historico].hit_rate = hit_rate;
        num_historico++;
    } else {
        // Deslocar
        for (int i = 1; i < MAX_HISTORICO; i++) {
            historico[i - 1] = historico[i];
        }
        historico[MAX_HISTORICO - 1].execucao = execucao_atual;
        historico[MAX_HISTORICO - 1].hits = hits;
        historico[MAX_HISTORICO - 1].misses = misses;
        historico[MAX_HISTORICO - 1].hit_rate = hit_rate;
    }
    execucao_atual++;

    // Calcular média
    double soma = 0.0;
    for (int i = 0; i < num_historico; i++) {
        soma += historico[i].hit_rate;
    }
    double media = (num_historico > 0) ? soma / num_historico : 0.0;
    printf("Média das últimas %d execuções: %.2f%%\n", num_historico, media);
}