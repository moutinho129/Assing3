/**
 * @file mock_task.h
 * @brief Cabeçalho para simulação de tarefas do sistema SETR Mini-Projeto.
 * * Define a estrutura de dados partilhada entre tarefas e declara mutexes para sincronização.
 * * Este arquivo é usado para testes unitários e simula o comportamento das tarefas do sistema.
 * 
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/06/01
 * @bug Desconhecidos
 */
 
#ifndef MOCK_TASK_H
#define MOCK_TASK_H

#include "mock_zephyr.h"

struct RTDB {
    int system_on;
    int max_temp;
    int target_temp;
    int adc_val;
    int current_temp;
    int Kp, Ti, Td;
};

extern struct k_mutex db_lock;
extern struct RTDB db;

#endif