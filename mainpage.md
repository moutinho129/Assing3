# Mini-Projeto SETR 2024/25

**Protótipo de um Sistema de Controlo Térmico**

---

## Objetivo

Desenvolver um protótipo de controlo térmico baseado em nRF52840-DK e Zephyr, com atuação por resistor de potência e leitura de temperatura via sensor digital I2C.

---

## Funcionalidades

- **Interface de Operação:**  
  - Botões para ligar/desligar o sistema e ajustar a temperatura.
  - LEDs para indicação do estado do sistema (ligado, temperatura normal, abaixo/acima do target).

- **Interface de Configuração (UART):**  
  - Permite definir parâmetros do sistema, limites de temperatura e obter medições em tempo real.
  - Protocolo de comandos robusto, com verificação por checksum e respostas ASCII.

- **Controlador:**  
  - On/Off ou PID, com parâmetros configuráveis via UART.

---

## Estrutura do Projeto

- `src/` – Código fonte principal (tarefas, controlador, etc)
- `include/` – Headers e interfaces
- `teste/` – Testes unitários e mocks
- `documentacao/` – Documentação Doxygen (auto-gerada)


---

## Autores

- **Nuno Moutinho** – 98626  
- **João Ferreira** - 98180

---

> Projeto realizado para a unidade curricular **Sistemas Embebidos e de Tempo-Real (SETR)**  
> Departamento de Eletrónica, Telecomunicações e Informática – Universidade de Aveiro


