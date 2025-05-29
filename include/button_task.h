/**
 * @file button_task.h
 * @brief Declaração da task responsável pela leitura dos botões físicos e atualização da RTDB.
 *
 * Esta task faz o polling dos botões e atualiza os estados do sistema
 * (ex: ON/OFF, incremento/decremento do setpoint) na RTDB, garantindo acesso concorrente seguro.
 *
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/05/29
 */

#ifndef BUTTON_TASK_H
#define BUTTON_TASK_H

/**
 * @brief Task responsável por monitorizar e tratar eventos dos botões físicos.
 * 
 *  Os botões são lidos periodicamente e os estados do sistema são atualizados na RTDB:
 *  *  - Botão 1: Liga/desliga o sistema
 *  *  - Botão 2: Incrementa a temperatura pretendida
 *  *  - Botão 3: Não utilizado
 *  *  - Botão 4: Decrementa a temperatura pretendida
 *
 * @param a Não utilizado
 * @param b Não utilizado
 * @param c Não utilizado
 */
void button_task(void *a, void *b, void *c);

#endif // BUTTON_TASK_H
