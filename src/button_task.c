/** 
 *  @file button_task.c
 *  @brief Funções usadas na task responsável pela leitura dos botões físicos e atualização da RTDB.
 *
 *  O ficheiro button_task.c contem a implementação das funções que foram declaradas no ficheiro button_task.h
 * 
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/05/29
 */

#include <zephyr/kernel.h>          
#include <zephyr/device.h>          
#include <zephyr/devicetree.h>		
#include <zephyr/drivers/gpio.h>  
#include <stdlib.h>  
#include "../include/task.h"

#define BTN1_NODE DT_ALIAS(sw0)
#define BTN2_NODE DT_ALIAS(sw1)
#define BTN4_NODE DT_ALIAS(sw3)

/**
 * @brief Tarefa responsável pela leitura dos botões físicos.
 *
 * Lê os botões definidos no sistema:
 * - Botão 1: Liga/desliga o sistema
 * - Botão 2: Aumenta o setpoint
 * - Botão 4: Diminui o setpoint
 *
 * Atualiza a RTDB com base nas interações do utilizador.
 *
 * @param a Ponteiro genérico (não utilizado)
 * @param b Ponteiro genérico (não utilizado)
 * @param c Ponteiro genérico (não utilizado)
 */

void button_task(void *a, void *b, void *c) {
    static const struct gpio_dt_spec buttons[] = {
        GPIO_DT_SPEC_GET(BTN1_NODE, gpios),
        GPIO_DT_SPEC_GET(BTN2_NODE, gpios),
        GPIO_DT_SPEC_GET(BTN4_NODE, gpios),
    };

    bool last_state[3] = {0}; 

    for (int index = 0; index < 3; index++) {
    const struct gpio_dt_spec *btn = &buttons[index];

    if (!device_is_ready(btn->port)) {
        printk("Falha ao inicializar o botão #%d (porta não pronta)\n", index);
        return;
    }
    // Configura o botão como entrada
    int ret = gpio_pin_configure_dt(btn, GPIO_INPUT);
    if (ret != 0) {
        printk("Erro ao configurar botão %d: %d\n", index, ret);
        return;
    }
}

    while (1) {
        for (int index = 0; index < 3; index++) {
            const struct gpio_dt_spec *btn = &buttons[index];
            bool atual = gpio_pin_get_dt(btn);
            
            if (atual != last_state[index]) {
                last_state[index] = atual;

                if (atual) {
                    printk(">> Ação no botão %d\n", index);
                    k_mutex_lock(&db_lock, K_FOREVER);

                    switch (index) {
                        // Botão 1: Liga/desliga o sistema
                        case 0:
                            db.system_on = !db.system_on;
                            break;
                        case 1:
                        // Botão 2: Aumenta a temperatura alvo
                            if (db.target_temp < db.max_temp)
                                db.target_temp++;
                            break;
                        case 2:
                        // Botão 4: Diminui a temperatura alvo
                            db.target_temp--;
                            break;
                        default:
                            break;
                    }
                    k_mutex_unlock(&db_lock);
                } else {
                    
                    printk(">> Botão %d libertado\n", index);
                }
            }
        }

        k_msleep(100);  
    } 
}
