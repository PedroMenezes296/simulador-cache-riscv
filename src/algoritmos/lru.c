#include "../../include/lru.h"
#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t valido;
    uint32_t tag;
    uint32_t idade;
} LinhaLRU;

static LinhaLRU cache_lru[NUM_CONJUNTOS][NUM_VIAS];
static LinhaLRU cache_L2_lru[L2_NUM_CONJUNTOS][L2_NUM_VIAS]; // Para simular a cache L2 unificada

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

// ==========================================
// FUNÇÕES AUXILIARES PARA L2 (Blocos de 64B)
// ==========================================
static uint32_t obter_indice_L2(uint32_t endereco) {
    return (endereco / L2_BLOCK_SIZE) % L2_NUM_CONJUNTOS;
}

static uint32_t obter_tag_L2(uint32_t endereco) {
    return endereco / (L2_BLOCK_SIZE * L2_NUM_CONJUNTOS);
}

static void atualizar_idade_L2(uint32_t indice, int via_acessada) {
    uint32_t idade_antiga = cache_L2_lru[indice][via_acessada].idade;
    for (int i = 0; i < L2_NUM_VIAS; i++) {
        if (i == via_acessada) {
            cache_L2_lru[indice][i].idade = 0;
        } else if (cache_L2_lru[indice][i].valido && cache_L2_lru[indice][i].idade < idade_antiga) {
            cache_L2_lru[indice][i].idade++;
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
    // Inicializa L2
    for (int i = 0; i < L2_NUM_CONJUNTOS; i++) {
        for (int j = 0; j < L2_NUM_VIAS; j++) {
            cache_L2_lru[i][j].valido = 0;
            cache_L2_lru[i][j].tag = 0;
            cache_L2_lru[i][j].idade = j;
        }
    }
    printf("[LRU] Caches L1 e L2 inicializadas.\n");
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
int acessar_L2_lru(uint32_t endereco) {
    uint32_t indice = obter_indice_L2(endereco);
    uint32_t tag = obter_tag_L2(endereco);

    // Tenta o Hit
    for (int i = 0; i < L2_NUM_VIAS; i++) {
        if (cache_L2_lru[indice][i].valido && cache_L2_lru[indice][i].tag == tag) {
            atualizar_idade_L2(indice, i);
            return 1;
        }
    }

    // Miss: Busca quem expulsar
    int via_substituir = 0;
    for (int i = 0; i < L2_NUM_VIAS; i++) {
        if (!cache_L2_lru[indice][i].valido) {
            via_substituir = i;
            break;
        }
        if (cache_L2_lru[indice][i].idade > cache_L2_lru[indice][via_substituir].idade) {
            via_substituir = i;
        }
    }

    cache_L2_lru[indice][via_substituir].valido = 1;
    cache_L2_lru[indice][via_substituir].tag = tag;
    atualizar_idade_L2(indice, via_substituir);
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
        printf("\n[LRU] === ESTADO ATUAL DA CACHE L2 ===\n");

        for (int i = 0; i < L2_NUM_CONJUNTOS; i++) {
            int conjunto_tem_dado = 0;
            for (int j = 0; j < L2_NUM_VIAS; j++) {
                if (cache_L2_lru[i][j].valido) {
                    conjunto_tem_dado = 1;
                    break;
                }
            }
            if (!conjunto_tem_dado) continue;

            printf("Conjunto %d:\n", i);
            for (int j = 0; j < L2_NUM_VIAS; j++) {
                printf("  Via %d | valido=%d | tag=0x%X | idade=%u\n",
                    j, cache_L2_lru[i][j].valido, cache_L2_lru[i][j].tag, cache_L2_lru[i][j].idade);
            }
        }
        printf("========================================\n");
    }
}