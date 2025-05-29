/** 
 *  @file task.c
 *  @brief Funções usadas nas tasks do sistema SETR Mini-Projeto.
 *
 *  O ficheiro task.c contem a implementação das funções que foram declaradas no ficheiro task.h
 * 
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/05/29
 */

#include "../include/task.h"
#include <stdlib.h>


struct RTDB db = {
    .system_on = 0,
    .max_temp = 30,      
};
struct k_mutex db_lock;