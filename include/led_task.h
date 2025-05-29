/**
 * @file led_task.h
 * @brief Declaração da task de controlo dos LEDs de estado do sistema.
 *
 * Esta task lê a RTDB e atualiza o estado dos LEDs consoante as condições de funcionamento
 *
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/05/29
 */

#ifndef LED_TASK_H
#define LED_TASK_H

/**
 * @brief Task responsável por atualizar os LEDs do sistema de acordo com a RTDB.
 *
 *  Os Leds são atualizados com base nos estados do sistema:
 *  *  - Led 1: (ON/OFF)
 *  *  - Led 2: Estado normal (temperatura +/- 2ºC da temperatura pretendida)
 *  *  - Led 3: Temperatura baixa (temperatura < 2ºC da temperatura pretendida)
 *  *  - Led 4: Temperatura alta (temperatura > 2ºC da temperatura pretendida)
 * 
 * @param a Não utilizado
 * @param b Não utilizado
 * @param c Não utilizado
 */

void led_task(void *a, void *b, void *c);

#endif // LED_TASK_H
