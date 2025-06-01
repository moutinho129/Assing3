/**
 * @file unitycmd.c
 * 
 * @brief Testes unitários para o módulo de comandos do sistema SETR Mini-Projeto.
 * * Este arquivo contém testes unitários para verificar o correto funcionamento dos comandos do sistema,
 * * incluindo o comando de definição de temperatura, configuração do PID e tratamento de erros.
 * 
 * * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * * @date 2025/06/01
 * * @bug Desconhecidos
 * 
 */
#include "unity.h"       
#include "mock_cmd.h"
#include "mock_task.h"      
#include "mock_zephyr.h"


void setUp(void) {}
void tearDown(void) {}
extern char txBuffer[]; 

// ----------------------------
// -- Teste para o comando M --
// ----------------------------
void test_M(void) {
    resetRxBuffer();
    // 10 ºC
    rxChar('#');
    rxChar('M');
    rxChar('0');
    rxChar('1');
    rxChar('0');
    //checksum
    rxChar('2');
    rxChar('2');
    rxChar('2');
    rxChar('!');

    int res = cmd();
    TEST_ASSERT_EQUAL_INT(CMD_SUCCESS, res);

    // Verificar se define a temperatura corretamente
    TEST_ASSERT_EQUAL(10, db.max_temp);  
    // Verificar a resposta
    TEST_ASSERT_EQUAL('#', UARTTxBuffer[0]);
    TEST_ASSERT_EQUAL('E', UARTTxBuffer[1]);
    TEST_ASSERT_EQUAL('o', UARTTxBuffer[2]);
}

// ----------------------------
// -- Teste para o checksum ---
// --------- errado -----------
// ----------------------------
void test_checksum_fail(void) {
    
    resetRxBuffer();
    rxChar('#');
    rxChar('M');
    rxChar('0');
    rxChar('1');
    rxChar('0');
    //checksum
    rxChar('0');
    rxChar('0');
    rxChar('0');
    rxChar('!');

    int res = cmd();
    TEST_ASSERT_EQUAL_INT(CMD_CS_ERROR,res);

    // Verificar a resposta
    TEST_ASSERT_EQUAL('#', UARTTxBuffer[0]);
    TEST_ASSERT_EQUAL('E', UARTTxBuffer[1]);
    TEST_ASSERT_EQUAL('s', UARTTxBuffer[2]);
}

// ----------------------------
// -- Teste para o comando C --
// ----------------------------
void test_C(void) {

    resetRxBuffer();
    rxChar('#');
    rxChar('C');
    //checksum
    rxChar('0');
    rxChar('6');
    rxChar('7');
    rxChar('!');
    
    int res = cmd();
    TEST_ASSERT_EQUAL_INT(CMD_SUCCESS,res);
}

void test_S(void) {
    resetRxBuffer();
    // Kp = 010, Ti = 050, Td = 100
    rxChar('#');
    rxChar('S');
    rxChar('0');
    rxChar('1');
    rxChar('0');
    rxChar('0');
    rxChar('5');
    rxChar('0');
    rxChar('1');
    rxChar('0');
    rxChar('0');
    //checksum
    rxChar('0');
    rxChar('1');
    rxChar('0');
    rxChar('!');
    
    int res = cmd();
    TEST_ASSERT_EQUAL_INT(CMD_SUCCESS,res);

    // Verificar a resposta
    TEST_ASSERT_EQUAL('#', UARTTxBuffer[0]);
    TEST_ASSERT_EQUAL('E', UARTTxBuffer[1]);
    TEST_ASSERT_EQUAL('o', UARTTxBuffer[2]);
}


// ----------------------------
// -- Teste para a string  ----
// -------- vazia -------------
// ----------------------------
void test_empty_string(void) {
    resetRxBuffer();
    int res = cmd();
    TEST_ASSERT_EQUAL_INT(CMD_EMPTY_STRING, res);
    
    // Verificar a resposta
    TEST_ASSERT_EQUAL('#', UARTTxBuffer[0]);
    TEST_ASSERT_EQUAL('E', UARTTxBuffer[1]);
    TEST_ASSERT_EQUAL('e', UARTTxBuffer[2]);
}

// ----------------------------
// -- Teste para o comando  ---
// -------- inválido ----------
// ----------------------------

void test_invalid_command(void) {
    resetRxBuffer();
    rxChar('#');
    rxChar('X'); // Invalid command
    rxChar('!');
    
    int res = cmd();
    TEST_ASSERT_EQUAL_INT(CMD_INVALID, res);

    // Verificar a resposta
    TEST_ASSERT_EQUAL('#', UARTTxBuffer[0]);
    TEST_ASSERT_EQUAL('E', UARTTxBuffer[1]);
    TEST_ASSERT_EQUAL('i', UARTTxBuffer[2]);
}

// ----------------------------
// -- Teste para o framming ---
// -------- inválido ----------
// ----------------------------
void test_framing_error(void) {
    resetRxBuffer();
    rxChar('#');
    rxChar('M');
    rxChar('0');
    rxChar('1');
    rxChar('0');
    
    int res = cmd();
    TEST_ASSERT_EQUAL_INT(CMD_FRAMING_ERROR, res);

    // Verificar a resposta
    TEST_ASSERT_EQUAL('#', UARTTxBuffer[0]);
    TEST_ASSERT_EQUAL('E', UARTTxBuffer[1]);
    TEST_ASSERT_EQUAL('f', UARTTxBuffer[2]);
    }

int main(void) {
    UNITY_BEGIN();
    printf("Iniciando os testes...\n");
    printf("\nTeste para o comando M:\n");
    RUN_TEST(test_M);
    printf("\nTeste para o comando M com falha de checksum:\n");
    RUN_TEST(test_checksum_fail);
    printf("\nTeste para o comando C:\n");
    RUN_TEST(test_C);
    printf("\nTeste para o comando S:\n");
    RUN_TEST(test_S);
    printf("\nTeste para string vazia:\n");
    RUN_TEST(test_empty_string);
    printf("\nTeste para comando inválido:\n");
    RUN_TEST(test_invalid_command);
    printf("\nTeste para erro de framing:\n");
    RUN_TEST(test_framing_error);
    printf("\nTodos os testes foram executados.\n");
    return UNITY_END();
}
