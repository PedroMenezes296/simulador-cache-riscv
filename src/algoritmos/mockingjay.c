#include "../../include/mockingjay.h"
#include <stdio.h>

void inicializar_cache_mockingjay() {
    // TODO: Inicializar estruturas do Mockingjay (RINF, contadores de tempo, etc)
    printf("[Mockingjay] Cache inicializada.\n");
}

int acessar_cache_mockingjay(uint32_t endereco) {
    // Lógica do colega responsável pelo Mockingjay 
    // O Mockingjay usa estimativa de tempo (ETR - Estimated Time of Reuse).
    
    // Simulação temporária:
    return 0; // Forçando Miss para o esqueleto
}

void imprimir_estado_mockingjay() {
    // Imprime as predições de intervalo para debugar
}