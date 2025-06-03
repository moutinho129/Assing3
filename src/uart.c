/**
 * @file uart.c
 * 
 * @brief Ficheiro responsável pela tarefa de comunicação UART.
 * Lê dados da UART de forma síncrona e executa comandos recebidos. 
 * Envia respostas de volta pela UART.
 * 
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/06/03
 * @bug comandos com mais de 7 caracteres não são lidos corretamente
 * 
 */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <string.h>
#include "../include/cmd.h"

#define UART_NODE DT_NODELABEL(uart0)
#define RXBUF_SIZE 128

static const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);

/**
 * @brief Tarefa de leitura da UART.
 * 
 * Lê dados da UART de forma síncrona, armazena os caracteres recebidos em um buffer
 * e executa comandos quando uma linha completa é recebida (terminada por '\n' ou '\r').
 * Respostas são enviadas de volta pela UART.
 * 
 */
void uart_task(void)
{
    uint8_t buf[RXBUF_SIZE];
    int idx = 0;
    uint8_t c;

    if (!device_is_ready(uart_dev)) {
        printk("UART device not ready!\n");
        return;
    }
    printk("UART polling task started\n");

    while (1) {
        if (uart_poll_in(uart_dev, &c) == 0) {
            if (c != '\n' && c != '\r' && idx < RXBUF_SIZE - 1) {
                buf[idx++] = c;
            }

            if (c == '\n' || c == '\r') {
                if (idx > 0) {
                    buf[idx] = 0;
                    cmd(buf, idx); 
                    printk("RESPOSTA: ");
                    for (int i = 0; i < txBufLen; i++)
                        printk("%c", UARTTxBuffer[i]);
                    printk("\n");
                }
                idx = 0; 
            }
        }
        k_msleep(10);
    }
}

