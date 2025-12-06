#include "usart_engine.h"

/* ====================================== GLOBAL VARIABES =========================================== */
lwrb_t esp_usart_rb;
/* ================================================================================================== */

/* ====================================== STATIC DECLARATIONS ======================================= */
static uint8_t lwrb_buffer[LWRB_BUFFER_SIZE]; 
/* ================================================================================================== */

void usart_sendstring(USART_TypeDef *USARTx, const char *str) {
    while (*str) {
        USART_SendData(USARTx, *str++);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}

void usart_engine_init(void){
    lwrb_init(&esp_usart_rb, lwrb_buffer, sizeof(lwrb_buffer));
}