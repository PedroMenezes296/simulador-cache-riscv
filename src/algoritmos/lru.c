#include "../../include/lru.h"
#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t valido;
    uint32_t tag;
    uint32_t idade;
} LinhaLRU;

static LinhaLRU cache_lru[NUM_CONJUNTOS][NUM_VIAS];

static uint32_t obter_indice(uint32_t endereco) {
    uint32_t bloco = endereco / BLOCK_SIZE_BYTES;
    return bloco % NUM_CONJUNTOS;
}

static uint32_t obter_tag(uint32_t endereco) {
    uint32_t bloco = endereco / BLOCK_SIZE_BYTES;
    return bloco / NUM_CONJUNTOS;
}

static void atualizar_lru(uint32_t indice, int via_acessada) {
    uint32_t idade_antiga = cache_lru[indice][via_acessada].idade;

    for (int i = 0; i < NUM_VIAS; i++) {
        if (i == via_acessada) {
            cache_lru[indice][i].idade = 0;
        } else if (cache_lru[indice][i].valido &&
                   cache_lru[indice][i].idade < idade_antiga) {
            cache_lru[indice][i].idade++;
        }
    }
}

void inicializar_cache_lru() {
    for (int i = 0; i < NUM_CONJUNTOS; i++) {
        for (int j = 0; j < NUM_VIAS; j++) {
            cache_lru[i][j].valido = 0;
            cache_lru[i][j].tag = 0;
            cache_lru[i][j].idade = j;
        }
    }

    printf("[LRU] Cache inicializada.\n");
}

int acessar_cache_lru(uint32_t endereco) {
    uint32_t indice = obter_indice(endereco);
    uint32_t tag = obter_tag(endereco);

    for (int i = 0; i < NUM_VIAS; i++) {
        if (cache_lru[indice][i].valido &&
            cache_lru[indice][i].tag == tag) {
            atualizar_lru(indice, i);
            return 1;
        }
    }

    int via_substituir = -1;

    for (int i = 0; i < NUM_VIAS; i++) {
        if (!cache_lru[indice][i].valido) {
            via_substituir = i;
            break;
        }
    }

    if (via_substituir == -1) {
        via_substituir = 0;

        for (int i = 1; i < NUM_VIAS; i++) {
            if (cache_lru[indice][i].idade > cache_lru[indice][via_substituir].idade) {
                via_substituir = i;
            }
        }
    }

    cache_lru[indice][via_substituir].valido = 1;
    cache_lru[indice][via_substituir].tag = tag;

    atualizar_lru(indice, via_substituir);

    return 0;
}

void imprimir_estado_lru() {
    printf("\n[LRU] Estado atual da cache:\n");

    for (int i = 0; i < NUM_CONJUNTOS; i++) {
        int conjunto_tem_dado = 0;

        for (int j = 0; j < NUM_VIAS; j++) {
            if (cache_lru[i][j].valido) {
                conjunto_tem_dado = 1;
                break;
            }
        }

        if (!conjunto_tem_dado) {
            continue;
        }

        printf("Conjunto %d:\n", i);

        for (int j = 0; j < NUM_VIAS; j++) {
            printf("  Via %d | valido=%d | tag=0x%X | idade=%u\n",
                   j,
                   cache_lru[i][j].valido,
                   cache_lru[i][j].tag,
                   cache_lru[i][j].idade);
        }
    }
}