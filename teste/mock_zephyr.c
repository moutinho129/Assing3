/**
 * @file mock_zephyr.c
 * 
 * @brief Ficheiro de simulação do Zephyr RTOS para testes unitários.
 * * Este ficheiro contém a implementação das funções de simulação do Zephyr RTOS, como mutexes e delays.
 * * É usado para testes unitários e simula o comportamento do Zephyr RTOS.
 * 
 * * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/06/01
 * @bug Desconhecidos
 */
 
#include "mock_zephyr.h"


// Funções de simulação de mutex
int k_mutex_lock(struct k_mutex *mutex, int timeout) {
    (void)mutex;
    (void)timeout;
    return 0;  // Simula sucesso
}

int k_mutex_unlock(struct k_mutex *mutex) {
    (void)mutex;
    return 0;
}






