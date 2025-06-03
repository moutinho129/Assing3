/**
 * @file temp_task.h
 * 
 * @brief Ficheiro de cabeçalho para a tarefa de leitura da temperatura do sensor TC74 via I2C.
 * * Define a função que lê a temperatura do sensor TC74 a cada segundo e atualiza a RTDB de forma segura com mutex.
 * 
 *  * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * * @date 2025/06/03
 * 
 */
#ifndef TEMP_TASK_H
#define TEMP_TASK_H


void temp_task(void *a, void *b, void *c);

#endif // TEMP_TASK_H
