/**
 * @file mock_task.c
 * 
 * @brief Ficheiro de criado para simular o comportamento das tasks do sistema SETR Mini-Projeto.
 * * Este ficheiro contém a implementação das funções que foram declaradas no ficheiro mock_task.h.
 * * Este ficheiro é idêntico ao ficheiro task.c, mas é usado para Unit Testing.
 * 
 * * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * * @date 2025/06/01
 * * @bug Desconhecidos
 */
#include "mock_task.h"
#include "mock_zephyr.h"
#include <stdlib.h>

/**
 * @struct RTDB
 * @brief Estrutura da base de dados partilhada entre tasks (RTDB).
 *
 * Contém os estados e parâmetros principais do sistema de controlo térmico.
 */
struct RTDB db = {
    .system_on = 0,
    .max_temp = 30,      
};

/** Mutex global para acesso concorrente à RTDB */
struct k_mutex db_lock;