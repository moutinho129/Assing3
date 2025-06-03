/**
 * @file main.c
 * @brief Ficheiro principal do projeto SETR Mini-Projeto.
 *
 * Responsável pela inicialização do sistema, mutex global e criação das threads para as tasks do sistema.
 *
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/05/29
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdlib.h>
#include "../include/task.h"
#include "../include/control_task.h"
#include "../include/button_task.h"
#include "../include/led_task.h"
#include "../include/cmd.h"
#include "../include/uart.h"
#include "../include/temp_task.h"



/** Cria e inicia a thread da tarefa de leitura dos botões */
K_THREAD_DEFINE(button_tid,512,button_task,NULL,NULL,NULL,5,0,0);

/** Cria e inicia a thread da tarefa de controlo dos LEDs */
K_THREAD_DEFINE(led_tid,512,led_task,NULL,NULL,NULL,5,0,0);

/** Cria e inicia a thread da tarefa de controlo da temperatura */
K_THREAD_DEFINE(temp_tid,   512, temp_task,   NULL, NULL, NULL, 5, 0,0);

/** Cria e inicia a thread da tarefa de controlo do sistema */
K_THREAD_DEFINE(ctrl_tid,   512, control_task,NULL, NULL, NULL, 5, 0, 0);

/** Cria e inicia a thread da tarefa de comunicação UART */
//K_THREAD_DEFINE(uart_tid, 512, uart_task, NULL, NULL, NULL, 5, 0, 0);




/**
 * @brief Função principal do sistema.
 *
 * Inicializa o mutex global e cria as threads para as tarefas do sistema:
 * - button_task: leitura dos botões físicos
 * - led_task: atualização dos LEDs de estado
 *
 * Entra num loop infinito com uma espera periódica de 1 segundo.
 */
void main(void)
{
    printk("Assign3: a correr! \n");
    init_rtdb();
    //uart_init();
    uart_task(); 
      while (1) {
        k_sleep(K_SECONDS(1000));   
    } 

    // Inicializa o mutex global para acesso concorrente à RTDB
    k_mutex_init(&db_lock);

}