/** 
 *  @file led_task.c
 *  @brief Funções usadas na task responsável por controlar os LEDs do sistema.
 *
 *  O ficheiro led_task.c contem a implementação das funções que foram declaradas no ficheiro led_task.h
 * 
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/05/29
 */


#include <zephyr/kernel.h>          
#include <zephyr/device.h>          
#include <zephyr/devicetree.h>		
#include <zephyr/drivers/gpio.h>  
#include <zephyr/sys/printk.h>
#include <stdlib.h>
#include "../include/task.h"


#define LED1_NODE DT_ALIAS(led0)
#define LED2_NODE DT_ALIAS(led1)
#define LED3_NODE DT_ALIAS(led2)
#define LED4_NODE DT_ALIAS(led3)

/**
 * @brief Tarefa responsável pela atualização dos LEDs do sistema.
 *
 * Atualiza os LEDs com base no estado atual do sistema, conforme definido na RTDB:
 * - LED1: Sistema ligado/desligado
 * - LED2: Estado normal (±2°C do setpoint)
 * - LED3: Temperatura abaixo do alvo
 * - LED4: Temperatura acima do alvo
 *
 * @param a Ponteiro genérico (não utilizado)
 * @param b Ponteiro genérico (não utilizado)
 * @param c Ponteiro genérico (não utilizado)
 */
void led_task(void *a, void *b, void *c) {
    static const struct gpio_dt_spec leds[] = {
        GPIO_DT_SPEC_GET(LED1_NODE, gpios),
        GPIO_DT_SPEC_GET(LED2_NODE, gpios),
        GPIO_DT_SPEC_GET(LED3_NODE, gpios),
        GPIO_DT_SPEC_GET(LED4_NODE, gpios),
    };

    for (int i = 0; i < 4; i++) {
        if (!device_is_ready(leds[i].port)) {
            printk("LED %d não está pronto!\n", i);
            return;
        }
        gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT);
        gpio_pin_set_dt(&leds[i], 0); // Começa tudo apagado!
    }

    while (1) {
    int diff = 0;
    int system_on, target, temp;
    k_mutex_lock(&db_lock, K_FOREVER);
    system_on = db.system_on;
    target = db.target_temp;
    temp = db.adc_val;
    k_mutex_unlock(&db_lock);

    gpio_pin_set_dt(&leds[0], system_on);

    if (!system_on) {
        gpio_pin_set_dt(&leds[1], 0);
        gpio_pin_set_dt(&leds[2], 0);
        gpio_pin_set_dt(&leds[3], 0);
    } else {
        diff = temp - target;
        // Primeiro desliga tudo
        gpio_pin_set_dt(&leds[1], 0);
        gpio_pin_set_dt(&leds[2], 0);
        gpio_pin_set_dt(&leds[3], 0);

        if (abs(diff) <= 2) {
            gpio_pin_set_dt(&leds[1], 1);
        } else if (diff < -2) {
            gpio_pin_set_dt(&leds[2], 1);
        } else if (diff > 2) {
            gpio_pin_set_dt(&leds[3], 1);
        }
    }
    k_msleep(200);
    printf("system_on=%d, target=%d, temp=%d, diff=%d\n", system_on, target, temp, diff);
}

}

