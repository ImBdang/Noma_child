#ifndef __USART_H__
#define __USART_H__

/* ====================================== INCLUDE HEADER =============================== */
#include "stm32f10x.h"
#include "debugger.h"
#include "lwrb.h"
/* ===================================================================================== */

/* ====================================== DEFINE ======================================= */
#define LWRB_BUFFER_SIZE            1280            /*<! BUFFER SIZE FOR LWRB TO RECEIVE DATA */
#define LINE_BUFFER_SIZE            1280            /*<! BUFFER SIZE FOR ARRAY GET DATA FROM LWRB TO PROCESS */
/* ===================================================================================== */


/* ====================================== API ========================================== */
void usart_sendstring(USART_TypeDef *USARTx, const char *str);
void usart_engine_init(void);
void esp32_parse(void);
/* ===================================================================================== */

#endif /* __USART_H__ */