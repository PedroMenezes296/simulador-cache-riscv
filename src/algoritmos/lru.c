#include "../../include/lru.h"
#include <stdio.h>

typedef struct {
    uint32_t tag;
    int valid;
} CacheLine;

CacheLine cache_lru[NUM_CONJUNTOS][NUM_VIAS];
int idades[NUM_CONJUNTOS][NUM_VIAS];

int calcular_conjunto_lru(uint32_t endereco) {
    return (endereco / BLOCK_SIZE_BYTES) % NUM_CONJUNTOS;
}

uint32_t calcular_tag_lru(uint32_t endereco) {
    return endereco / (BLOCK_SIZE_BYTES * NUM_CONJUNTOS);
}

void inicializar_cache_lru() {
    for (int i = 0; i < NUM_CONJUNTOS; i++) {
        for (int j = 0; j < NUM_VIAS; j++) {
            cache_lru[i][j].valid = 0;
            cache_lru[i][j].tag = 0;
            idades[i][j] = 0;
        }
    }
    printf("[LRU] Cache inicializada.\n");
}

int acessar_cache_lru(uint32_t endereco) {
    int conjunto = calcular_conjunto_lru(endereco);
    uint32_t tag = calcular_tag_lru(endereco);

    // Verificar hit
    for (int v = 0; v < NUM_VIAS; v++) {
        if (cache_lru[conjunto][v].valid && cache_lru[conjunto][v].tag == tag) {
            // Hit, atualizar idade
            idades[conjunto][v] = 0; // Reset to most recent
            for (int k = 0; k < NUM_VIAS; k++) {
                if (k != v) idades[conjunto][k]++;
            }
            return 1;
        }
    }

    // Miss
    int via_substituir = 0;
    int max_idade = -1;
    for (int v = 0; v < NUM_VIAS; v++) {
        if (!cache_lru[conjunto][v].valid) {
            via_substituir = v;
            break;
        }
        if (idades[conjunto][v] > max_idade) {
            max_idade = idades[conjunto][v];
            via_substituir = v;
        }
    }

    // Substituir
    cache_lru[conjunto][via_substituir].tag = tag;
    cache_lru[conjunto][via_substituir].valid = 1;
    idades[conjunto][via_substituir] = 0;
    for (int k = 0; k < NUM_VIAS; k++) {
        if (k != via_substituir) idades[conjunto][k]++;
    }
    return 0;
}

void imprimir_estado_lru(uint32_t endereco) {
    int conjunto = calcular_conjunto_lru(endereco);
    printf("| Cache: ");
    for (int v = 0; v < NUM_VIAS; v++) {
        if (cache_lru[conjunto][v].valid) {
            printf("%c", 'A' + cache_lru[conjunto][v].tag);
        } else {
            printf("-");
        }
        if (v < NUM_VIAS - 1) printf(", ");
    }
    printf("\n");
}