#include "../../include/lru.h"
#include <stdio.h>

void inicializar_cache_lru() {
    // TODO: Inicializar estruturas de dados do LRU (ex: matriz de tags e idades)
    printf("[LRU] Cache inicializada.\n");
}

int acessar_cache_lru(uint32_t endereco) {
    // Lógica do colega responsável pelo LRU
    // 1. Calcular índice e tag
    // 2. Buscar nas vias do conjunto
    // 3. Se hit, atualiza idade. Retorna 1.
    // 4. Se miss, encontra a via mais velha (Least Recently Used), substitui. Retorna 0.
    
    // Simulação temporária:
    return 0; // Forçando Miss para o esqueleto
}

void imprimir_estado_lru() {
    // Imprime o estado atual dos conjuntos para debugar passo a passo
}