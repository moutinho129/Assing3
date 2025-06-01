/**
 * @file cmd.h
 * 
 * @brief Ficheiro de cabeçalho para teste dos comandos do sistema SETR Mini-Projeto.
 * * Este ficheiro contém as declarações das funções e variáveis usadas para testar os comandos do sistema.
 * * #Mxxxyyy! - Define a temperatura máxima do sistema (xxx é o valor da temperatura máxima, yyy é o checksum).
 * * #Cyyy! - Lê a temperatura atual do sensor (retorna a temperatura atual, seguida do checksum).
 * * #Sxxxxyyy! - Define os parâmetros do PID (Kp, Ti, Td) do sistema (xxx são os parametros (Kp, Ti, Td), yyy é o checksum).
 * 
 * * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * * @date 2025/06/01
 * * @bug Desconhecidos
 * 
 */

#ifndef CMD_PROC_H_
#define CMD_PROC_H_

#define UART_RX_SIZE 20 	/* Maximum size of the RX buffer */ 
#define UART_TX_SIZE 20 	/* Maximum size of the TX buffer */ 
#define BUFFER_SIZE 20 	/* Maximum size of the history buffers */
#define START '#'	        /* Start of Frame Symbol */
#define END '!'          /* End of Frame Symbol */

/* *********************************************************************************** */
#define CMD_SUCCESS 0         
#define CMD_EMPTY_STRING -1  
#define CMD_INVALID -2        
#define CMD_CS_ERROR -3        
#define CMD_WRONG_FORMAT -4    
#define CMD_FRAMING_ERROR -5 
  

/* *********************************************************************************** */

/* Used as part of the UART emulation */
/**
 * @brief Buffer de recepção UART.
 * * Este buffer é usado para armazenar os dados recebidos pela UART.
 */
extern unsigned char UARTRxBuffer[UART_RX_SIZE];
/**
 * @brief Comprimento atual do buffer de recepção UART.
 * * Indica o número de bytes atualmente armazenados no buffer de recepção UART.
 */
extern unsigned char rxBufLen; 

/**
 * @brief Buffer de transmissão UART.
 * * Este buffer é usado para armazenar os dados a serem enviados pela UART.
 */
extern unsigned char UARTTxBuffer[UART_TX_SIZE];

/**
 * @brief Comprimento atual do buffer de transmissão UART.
 * * Indica o número de bytes atualmente armazenados no buffer de transmissão UART.
 */
extern unsigned char txBufLen; 


/**
 * @brief Função para processar comandos recebidos pela UART.
 * * Esta função analisa os comandos recebidos no buffer de recepção UART e executa as ações correspondentes.
 * 
 */
int cmd(void);

/**
 * @brief Envia um caractere para o buffer de recepção UART.
 * * Esta função adiciona um caractere ao buffer de recepção UART, se o buffer não estiver cheio.
 * @param car O caractere a ser recebido.
 * @return int Retorna 0 se o caractere foi adicionado com sucesso, ou -1 se o buffer estiver cheio.
 * 
 */
int rxChar(unsigned char car);

/**
 * @brief Envia um caractere para o buffer de transmissão UART.
 * * Esta função adiciona um caractere ao buffer de transmissão UART, se o buffer não estiver cheio.
 * @param car O caractere a ser enviado.
 * @return int Retorna 0 se o caractere foi adicionado com sucesso, ou -1 se o buffer estiver cheio.
 * 
 */
int txChar(unsigned char car);

/**
 * @brief Reseta o buffer de recepção UART.
 * 
 * Esta função limpa o buffer de recepção UART, definindo seu comprimento para zero.
 * 
 */
void resetRxBuffer(void);

/**
 * @brief Reseta o buffer de transmissão UART.
 * 
 * Esta função limpa o buffer de transmissão UART, definindo seu comprimento para zero.
 * 
 */
void resetTxBuffer(void);

/**
 * @brief Define a temperatura máxima do sistema.
 * 
 * Esta função atualiza a temperatura máxima permitida no sistema.
 * 
 */
void getTxBuffer(unsigned char *buf, int *len);

/**
 * @brief Calcula o checksum de um buffer.
 * 
 * Esta função calcula o checksum de um buffer de bytes e verifica se o checksum está correto.
 * 
 * @param buf O buffer de bytes para calcular o checksum.
 * @param nbytes O número de bytes no buffer.
 * @return int Retorna CMD_SUCCESS se o checksum estiver correto, ou CMD_CS_ERROR se estiver incorreto.
 */
int calcChecksum(unsigned char *buf, int nbytes);

/**
 * @brief Converte um número inteiro em uma representação ASCII.
 * * Esta função converte um número inteiro em uma string ASCII, desde que o número esteja no intervalo de 1 a 99999.
 * @param num O número inteiro a ser convertido.
 * @param str O buffer onde a representação ASCII será armazenada.
 * * Esta função verifica se o número é negativo e o converte para positivo antes de a conversão.
 * * Se o número estiver fora do intervalo permitido, a string resultante será vazia.
 * 
 */
void intToAscii(int num, char *str);

#endif