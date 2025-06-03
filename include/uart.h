/**
 * @file uart.h
 * 
 * @brief Ficheiro de cabeçalho para a tarefa de comunicação UART.
 * * Define a função de leitura da UART que processa comandos recebidos e envia respostas.
 * 
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/06/03
 * @bug Comandos com mais de 7 caracteres não são lidos corretamente
 * 
 */
#ifndef UART_H
#define UART_H
#include <zephyr/drivers/uart.h>


/**
 * @brief Tarefa de leitura da UART.
 * * Lê dados da UART de forma síncrona, armazena os caracteres recebidos em um buffer
 * * e executa comandos quando uma linha completa é recebida (terminada por '\n' ou '\r').
 * * Respostas são enviadas de volta pela UART.
 * 
 */
void uart_task(void);


#endif
