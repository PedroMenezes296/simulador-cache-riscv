#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>

// Estrutura para guardar os resultados lógicos da simulação 
typedef struct {
    int hits;
    int misses;
    int acessos_totais;
} CacheStats;

// Definições básicas para iniciar os testes (podem ser parametrizadas depois)
#define NUM_VIAS 4
#define NUM_CONJUNTOS 16 

#endif