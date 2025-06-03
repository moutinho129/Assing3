/**
 * @file control_task.h
 * 
 * @brief Ficheiro de cabeçalho para a tarefa de controlo térmico.
 * * Define a função de controlo que compara a temperatura atual com o setpoint
 * e ativa ou desativa o aquecedor (resistor de potência) consoante a lógica de controlo definida.
 * 
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/06/03
 * 
 */
#ifndef CONTROL_TASK_H
#define CONTROL_TASK_H

#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Tarefa de controlo térmico.
 *
 * Compara a temperatura atual com o setpoint e ativa ou desativa o aquecedor (resistor de potência)
 * consoante a lógica de controlo definida (On/Off com histerese ou PID).
 *
 * @param a Parâmetro genérico (não utilizado)
 * @param b Parâmetro genérico (não utilizado)
 * @param c Parâmetro genérico (não utilizado)
 */
void control_task(void);
#ifdef __cplusplus
}
#endif

#endif // CONTROL_TASK_H