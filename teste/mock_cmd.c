/** 
 * @file mock_cmd.c
 * 
 * @brief Ficheiro para teste dos comandos do sistema SETR Mini-Projeto.
 * 
 * Este ficheiro contém a implementação das funções que foram declaradas no ficheiro mock_cmd.h.
 * Ficheiro identico ao ficheiro cmd.c mas usado para Unit Testing. 
 * 
 * * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * * @date 2025/06/01
 * * @bug Desconhecidos
 */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "mock_cmd.h"
#include "mock_zephyr.h"
#include "mock_task.h"

/**
 * @brief Tamanho máximo do buffer de transmissão UART.
 * 
 * Define o tamanho máximo do buffer de transmissão UART.
 */
unsigned char UARTTxBuffer[UART_TX_SIZE];

/**
 * @brief Tamanho atual do buffer de transmissão UART.
 * 
 * Indica o número de bytes atualmente armazenados no buffer de transmissão UART.
 */
unsigned char txBufLen = 0;

/**
 * @brief Tamanho máximo do buffer de recepção UART.
 * 
 * Define o tamanho máximo do buffer de recepção UART.
 */
unsigned char UARTRxBuffer[UART_RX_SIZE];
/**
 * @brief Tamanho atual do buffer de recepção UART.
 * 
 * Indica o número de bytes atualmente armazenados no buffer de recepção UART.
 */
unsigned char rxBufLen = 0;


/**
 * @brief Define a temperatura máxima do sistema.
 * 
 * Esta função atualiza a temperatura máxima permitida no sistema.
 * 
 */
// função para definir a temperatura
void set_max_temp(int t) {
    k_mutex_lock(&db_lock, K_FOREVER);
    db.max_temp = t;
    k_mutex_unlock(&db_lock);
}

/**
 * @brief Lê a temperatura atual do sensor.
 * 
 * Esta função obtém a temperatura atual medida pelo sensor.
 * 
 * @return int A temperatura atual.
 */
// função para ler a temperatura atual do sensor
int get_current_temp(void) {
    int val;
    k_mutex_lock(&db_lock, K_FOREVER);
    val = db.adc_val;  // ou outro campo para temperatura atual!
    k_mutex_unlock(&db_lock);
    return val;
}

int cmd(void){

    int i = 0;
    char tempStr[4]; 
    int cs; 

    // deteta se o buffer está vazio
	if(rxBufLen == 0){

		resetTxBuffer();
        txChar('#'); txChar('E'); txChar('e');
        cs = ('E' + 'e') % 256;
        txChar((cs / 100) % 10 + '0');
        txChar((cs / 10) % 10 + '0');
        txChar((cs % 10) + '0');
        txChar('!');
        txBufLen = 7;
        resetRxBuffer();		
		
		return CMD_EMPTY_STRING; 
	};

	// deteta erros de framing
	if (UARTRxBuffer[0] != START || UARTRxBuffer[rxBufLen - 1] != END) {
		
		resetTxBuffer();
        txChar('#'); txChar('E'); txChar('f');
        cs = ('E' + 'f') % 256;
        txChar((cs / 100) % 10 + '0');
        txChar((cs / 10) % 10 + '0');
        txChar((cs % 10) + '0');
        txChar('!');
        txBufLen = 7;
        resetRxBuffer();
		
		return CMD_FRAMING_ERROR;
	}

	/* Find index of SOF */
	for(i=0; i < rxBufLen; i++) {
		if(UARTRxBuffer[i] =START) {
			break;
		}
	}

    if(i < rxBufLen) {
		
	switch(UARTRxBuffer[i+1]) { 

		//---------------------------------------------------
        // --------------------- caso 'M' -------------------
		// --------------------------------------------------
        case 'M':

			// Verifica se o checksum está correto
			if(calcChecksum(&(UARTRxBuffer[i+1]), 4) != CMD_SUCCESS){
						resetTxBuffer();
						txChar('#'); txChar('E'); txChar('s');
						cs = ('E' + 's') % 256;
						txChar((cs / 100) % 10 + '0');
						txChar((cs / 10) % 10 + '0');
						txChar((cs % 10) + '0');
						txChar('!');
						txBufLen = 7;
						resetRxBuffer();
						return CMD_CS_ERROR;
					}

				tempStr[0] = UARTRxBuffer[i+2];
				tempStr[1] = UARTRxBuffer[i+3];
				tempStr[2] = UARTRxBuffer[i+4];
				tempStr[3] = '\0';

                int newMax = atoi(tempStr);
                set_max_temp(newMax);

				// resposta
				resetTxBuffer();
                txChar('#'); txChar('E'); txChar('o');
				//checksum
                cs = ('E' + 'o') % 256;
                txChar((cs / 100) % 10 + '0');
                txChar((cs / 10) % 10 + '0');
                txChar((cs % 10) + '0');
                txChar('!');
                txBufLen = 7;
                resetRxBuffer();
                return CMD_SUCCESS;
            
        break;
        

        // ---------------------------------------------------
        // --------------------- caso 'C' --------------------
		// --------------------------------------------------
        case 'C': 
			
			// Verifica se o checksum está correto
			if(calcChecksum(&(UARTRxBuffer[i+1]), 1) != CMD_SUCCESS){
				resetTxBuffer();
                txChar('#'); txChar('E'); txChar('s');
                int cs = ('E' + 's') % 256;
                txChar((cs / 100) % 10 + '0');
                txChar((cs / 10) % 10 + '0');
                txChar((cs % 10) + '0');
                txChar('!');
                txBufLen = 7;
                resetRxBuffer();
                return CMD_CS_ERROR;
            }

			// resposta
			resetTxBuffer();
			txChar('#');
			txChar('C');
			
			intToAscii(get_current_temp(), tempStr);
			txChar(tempStr[0]);
			txChar(tempStr[1]);
			txChar(tempStr[2]);
			
			//checksum
			cs = ('c' + tempStr[0] + tempStr[1] + tempStr[2]) % 256;
			txChar((cs / 100) % 10 + '0');
			txChar((cs / 10) % 10 + '0');
			txChar((cs % 10) + '0');
			txChar('!');
			txBufLen = 9;
			resetRxBuffer();
			return CMD_SUCCESS;

		break;
        
        
		// --------------------------------------------------
		// ------------------ caso 'S'	---------------------
		// -------------------------------------------------- 
        case 'S':
		if (rxBufLen >= 15 && calcChecksum(&(UARTRxBuffer[i+1]), 10) == CMD_SUCCESS) {
        	// Extrair Kp, Ti, Td
        	char kpStr[4] = { UARTRxBuffer[i+2], UARTRxBuffer[i+3], UARTRxBuffer[i+4], '\0' };
        	char tiStr[4] = { UARTRxBuffer[i+5], UARTRxBuffer[i+6], UARTRxBuffer[i+7], '\0' };
        	char tdStr[4] = { UARTRxBuffer[i+8], UARTRxBuffer[i+9], UARTRxBuffer[i+10], '\0' };

			// converte os valores de string para inteiro
        	int kp = atoi(kpStr);
        	int ti = atoi(tiStr);
        	int td = atoi(tdStr);

        	k_mutex_lock(&db_lock, K_FOREVER);
        	db.Kp = kp;
        	db.Ti = ti;
        	db.Td = td;
        	k_mutex_unlock(&db_lock);

        	// Resposta de sucesso
        	resetTxBuffer();
        	txChar('#'); txChar('E'); txChar('o');
			//checksum
        	cs = ('E' + 'o') % 256;
        	txChar((cs / 100) % 10 + '0');
        	txChar((cs / 10) % 10 + '0');
        	txChar((cs % 10) + '0');
        	txChar('!');
        	txBufLen = 7;
        	resetRxBuffer();
        	return CMD_SUCCESS;
    	}
        break;
		
		default:
                resetTxBuffer();
				// resposta de erro
                txChar('#'); txChar('E'); txChar('i');
                cs = ('E' + 'i') % 256;
                txChar((cs / 100) % 10 + '0');
                txChar((cs / 10) % 10 + '0');
                txChar((cs % 10) + '0');
                txChar('!');
                txBufLen = 7;
                resetRxBuffer();
                return CMD_INVALID;
        }
    }	
	
	/* Cmd string not null and SOF not found */
	return CMD_WRONG_FORMAT;
}


// ------------------------------------------------------
// ---------------------calcChecksum --------------------
// ------------------------------------------------------
/**
 * @brief Calcula o checksum de um buffer de bytes.
 * 
 * Esta função calcula o checksum de um buffer de bytes e verifica se o checksum calculado corresponde ao checksum esperado.
 * @param buf Ponteiro para o buffer de bytes.
 * @param nbytes Número de bytes no buffer.
 * @return int Retorna CMD_SUCCESS se o checksum estiver correto, ou CMD_CS_ERROR se houver um erro no checksum.
 */
int calcChecksum(unsigned char * buf, int nbytes) {
	char checksumAscii[4]; 
	int checksum = 0;
    for (int i = 0; i < nbytes; i++) checksum += buf[i];
    checksum %= 256;
    checksumAscii[0] = (checksum / 100) % 10 + '0';
    checksumAscii[1] = (checksum / 10) % 10 + '0';
    checksumAscii[2] = checksum % 10 + '0';
    checksumAscii[3] = '\0';

    if (buf[nbytes]   == checksumAscii[0] &&
        buf[nbytes+1] == checksumAscii[1] &&
        buf[nbytes+2] == checksumAscii[2]) {
        return CMD_SUCCESS; 
    } else {
        return CMD_CS_ERROR; 
    }
}

// ------------------------------------------------------------
// ------------------ intToAscii ------------------------------
// ------------------------------------------------------------
/**
 * @brief Converte um número inteiro em uma representação ASCII.
 * * Esta função converte um número inteiro em uma string ASCII, desde que o número esteja no intervalo de 1 a 99999.
 * @param num O número inteiro a ser convertido.
 * @param str O buffer onde a representação ASCII será armazenada.
 * * Esta função verifica se o número é negativo e o converte para positivo antes de a conversão.
 * * Se o número estiver fora do intervalo permitido, a string resultante será vazia.
 * 
 */
void intToAscii(int num, char *str) {
	if(num <0){
		num = -num;
	}
    if (num <= 0 || num >= 99999) {
        str[0] = '\0';
        return;
    }
    sprintf(str, "%d", num);
}
// ---------------------------------------------------------------
// ---rxChar, txChar, resetRxBuffer, resetTxBuffer, getTxBuffer --
// ---------------------------------------------------------------
/*
 * rxChar
 */
/**
 * @brief Envia um caractere para o buffer de recepção UART.
 * * Esta função adiciona um caractere ao buffer de recepção UART, se o buffer não estiver cheio.
 * @param car O caractere a ser recebido.
 * @return int Retorna 0 se o caractere foi adicionado com sucesso, ou -1 se o buffer estiver cheio.
 * 
 */
int rxChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (rxBufLen < UART_RX_SIZE) {
		UARTRxBuffer[rxBufLen] = car;
		rxBufLen += 1;
		return 0;		
	}	
	/* If cmd string full return error */
	return -1;
}

/*
 * txChar
 */
/**
 * @brief Envia um caractere para o buffer de transmissão UART.
 * * Esta função adiciona um caractere ao buffer de transmissão UART, se o buffer não estiver cheio.
 * @param car O caractere a ser enviado.
 * @return int Retorna 0 se o caractere foi adicionado com sucesso, ou -1 se o buffer estiver cheio.
 */

int txChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (txBufLen < UART_TX_SIZE) {
		UARTTxBuffer[txBufLen] = car;
		txBufLen += 1;
		return 0;		
	}	
	/* If cmd string full return error */
	return -1;
}

/*
 * resetRxBuffer
 */

 /**
  * @brief Reseta o buffer de recepção UART.
  * 
  * Esta função limpa o buffer de recepção UART, definindo seu comprimento para zero.
  * 
  */
    void resetRxBuffer(void)
{
	rxBufLen = 0;		
	return;
}

/*
 * resetTxBuffer
 */

 /**
  * @brief Reseta o buffer de transmissão UART.
  * 
  * Esta função limpa o buffer de transmissão UART, definindo seu comprimento para zero.
  * 
  */
void resetTxBuffer(void)
{
	txBufLen = 0;		
	return;
}

/*
 * getTxBuffer
 */

 /**
  * @brief Obtém o conteúdo do buffer de transmissão UART.
  * 
  * Esta função copia o conteúdo do buffer de transmissão UART para o buffer fornecido e atualiza o comprimento do buffer.
  * @param buf Ponteiro para o buffer onde o conteúdo será copiado.
  * @param len Ponteiro para um inteiro onde o comprimento do buffer será armazenado.
  * 
  */
void getTxBuffer(unsigned char * buf, int * len)
{
	*len = txBufLen;
	if(txBufLen > 0) {
		memcpy(buf,UARTTxBuffer,*len);
	}		
	return;
}


