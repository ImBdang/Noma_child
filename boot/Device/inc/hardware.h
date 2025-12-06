#ifndef __HARDWARE_H__
#define __HARDWARE_H__

/* ====================================== INCLUDE HEADER =============================== */
#include "stm32f10x.h"
#include "systick.h"
/* ===================================================================================== */

/* ====================================== DEFINE ======================================= */


/* ================================================================================================== */
#define USART_DEBUGGER_PORT            USART2
#define USART_DEBUGGER_BAUD            115200

#define USART_DEBUGGER_GPIO_PORT       GPIOA
#define USART_DEBUGGER_TX_PIN          GPIO_Pin_2
#define USART_DEBUGGER_RX_PIN          GPIO_Pin_3

#define USART_DEBUGGER_TX_PIN_SOURCE   GPIO_PinSource2
#define USART_DEBUGGER_RX_PIN_SOURCE   GPIO_PinSource3

#define USART_DEBUGGER_TX_AF           GPIO_AF_USART2
#define USART_DEBUGGER_RX_AF           GPIO_AF_USART2

#define USART_DEBUGGER_RCC_GPIO        RCC_APB2Periph_GPIOA
#define USART_DEBUGGER_RCC_USART       RCC_APB1Periph_USART2
/* ================================================================================================== */


/* ===================================================================================== */


/* ====================================== API ========================================== */
void hardware_init(void);
/* ===================================================================================== */

#endif /* __HARDWARE_H__ */