#include "usart_engine.h"

/* ====================================== GLOBAL VARIABES =========================================== */
/* ================================================================================================== */

/* ====================================== STATIC DECLARATIONS ======================================= */
/* ================================================================================================== */

void usart_sendstring(USART_TypeDef *USARTx, const char *str) {
    while (*str) {
        USART_SendData(USARTx, *str++);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}