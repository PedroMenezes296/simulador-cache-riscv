# Simulador de Caches Inteligentes em RISC-V (LRU vs Mockingjay)

Projeto desenvolvido para a disciplina de Projeto Integrador IV, no semestre 2026/1, sob a orientação do Prof. Bruno S. Neves.

O objetivo deste projeto é implementar e avaliar algoritmos de substituição de blocos em cache, comparando o desempenho do algoritmo padrão LRU com o algoritmo baseado em heurística Mockingjay.

## Estrutura do Projeto (Fase 1 - Modelagem em C)

Nesta fase inicial (Semanas 1-4), o foco é a modelagem em software para validação do *Hit Rate* lógico.

- `/include`: Arquivos de cabeçalho (`.h`) com as estruturas de dados e assinaturas.
- `/src/main.c`: Orquestrador do simulador que lê os arquivos de trace.
- `/src/algoritmos/lru.c`: Implementação da política *Least Recently Used*.
- `/src/algoritmos/mockingjay.c`: Implementação da política baseada em estimativa de tempo de reuso (Mockingjay).
- `/traces`: Arquivos de texto contendo sequências de endereços de memória para testes e validação.

## Como Compilar e Executar (Windows)

### Opção 1: Usando o Makefile (Recomendado)

O projeto conta com um `Makefile` para automatizar a compilação. No Windows, se você utiliza o MinGW, o comando correspondente é o `mingw32-make`.

1. **Verifique a instalação:**
   ```powershell
   mingw32-make --version
Compilação: PowerShell
        -> mingw32-make
    Dica: Atalho para o comando make
    Para facilitar, você pode criar um apelido (alias) no   PowerShell para usar apenas make:
        -> Set-Alias -Name make -Value mingw32-make
        -> make

3. Opção 2: Compilação Direta via GCC
Caso não utilize o Make, você pode compilar manualmente todos os arquivos fonte:

PowerShell
gcc src/main.c src/algoritmos/lru.c src/algoritmos/mockingjay.c -o simulador_cache.exe