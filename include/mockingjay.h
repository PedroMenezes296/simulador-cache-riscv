// include/mockingjay.h
#ifndef MOCKINGJAY_H
#define MOCKINGJAY_H
#include "cache.h"
#include <stdint.h>

void inicializar_cache_mockingjay();
int acessar_cache_mockingjay(uint32_t endereco); // Retorna 1 para Hit, 0 para Miss
void imprimir_estado_mockingjay(); // Útil para validação linha a linha
void set_trace_mockingjay(uint32_t *t, int size);
void finalizar_execucao_mockingjay(int hits, int misses);

#endif