/**
 * @file mock_zephyr.h
 * 
 * @brief Cabeçalho para simulação do Zephyr RTOS em testes unitários.
 * * Este arquivo define constantes e estruturas simuladas do Zephyr RTOS para testes unitários.
 * * * É usado para simular o comportamento do Zephyr RTOS, como mutexes e delays, sem depender do ambiente real.
 * 
 * * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * * @date 2025/06/01
 * * @bug Desconhecidos
 * 
 */
#ifndef MOCK_ZEPHYR_H
#define MOCK_ZEPHYR_H

// Substituto da constante do Zephyr
#define K_FOREVER 0

// Estrutura vazia para simular o Zephyr
struct k_mutex {
    int dummy;
};

// Funções simuladas do Zephyr
int k_mutex_lock(struct k_mutex *mutex, int timeout);
int k_mutex_unlock(struct k_mutex *mutex);


#endif
