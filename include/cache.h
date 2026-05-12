#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>

typedef struct {
    int hits;
    int misses;
    int acessos_totais;
} CacheStats;

// --- CONFIGURAÇÕES DA CACHE L1 (Baseado na Especificação) ---
#define CACHE_CAPACITY_BYTES 4096 // 4 KB
#define BLOCK_SIZE_BYTES     32   // 32 Bytes
#define NUM_VIAS             2    // Associatividade: 2-vias
// O compilador calcula os conjuntos automaticamente para nós!
#define NUM_CONJUNTOS        (CACHE_CAPACITY_BYTES / (BLOCK_SIZE_BYTES * NUM_VIAS))

// --- CONFIGURAÇÕES DA CACHE L2 (Unificada) ---
#define L2_CAPACITY_BYTES 32768 // 32 KB
#define L2_BLOCK_SIZE     64    // 64 Bytes
#define L2_NUM_VIAS       8     // 8-vias
#define L2_NUM_CONJUNTOS  (L2_CAPACITY_BYTES / (L2_BLOCK_SIZE * L2_NUM_VIAS))

#endif