/** 
 *  @file task.c
 *  @brief Funções usadas nas tasks do sistema SETR Mini-Projeto.
 *
 *  O ficheiro task.c contem a implementação das estruturas que foram declaradas no ficheiro task.h
 * 
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/05/29
 */

#include "../include/task.h"
#include <stdlib.h>


struct RTDB db;
struct k_mutex db_lock;

void init_rtdb(void) {
        k_mutex_init(&db_lock);  // Inicializa o mutex

    // Valores iniciais do sistema
    db.system_on = 0;          // Sistema desligado por defeito
    db.max_temp = 30;          // Temperatura máxima configurável
    db.target_temp = 25;       // Temperatura desejada default
    db.i2c_val= 20;           // Valor do sensor ADC no arranque
    db.current_temp = 0;       // Temperatura medida inicial
    db.hysteresis = 2;         // Histerese default para ON/OFF
}