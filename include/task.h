/**
 * @file task.h
 * @brief Definição da RTDB (Real-Time DataBase) e funções utilitárias para tasks.
 *
 * Define a estrutura partilhada entre tasks para sincronização do estado do sistema.
 * Inclui variáveis globais, mutex e protótipos para acesso e manipulação da RTDB.
 *
* @author Nuno Moutinho - 98626 e João Ferreira - 98180
 *  @date 2025/05/29
 */

 #ifndef TASK_H
#define TASK_H

#include <zephyr/kernel.h>

/**
 * @struct RTDB
 * @brief Estrutura da base de dados partilhada entre tasks (RTDB).
 *
 * Contém os estados e parâmetros principais do sistema de controlo térmico.
 */
struct RTDB {
    int system_on;      
    int max_temp;       
    int target_temp;  
    int adc_val;        
    int current_temp;  
};


/** Variável global da RTDB partilhada entre as tasks */
extern struct RTDB db;


/** Mutex global para acesso concorrente à RTDB */
extern struct k_mutex db_lock;


#endif
