#ifndef __HARDWARE_H__
#define __HARDWARE_H__

/* ====================================== INCLUDE HEADER =============================== */
#include "stm32f10x.h"
#include "systick.h"
/* ===================================================================================== */

/* ====================================== DEFINE ======================================= */
/* ================================================================================================== */
#define USART_ESP                      USART1
#define USART_ESP_BAUD                 115200
    
#define USART_ESP_GPIO_PORT            GPIOA
#define USART_ESP_TX_PIN               GPIO_Pin_9
#define USART_ESP_RX_PIN               GPIO_Pin_10
    
#define USART_ESP_TX_AF                GPIO_AF_USART1
#define USART_ESP_RX_AF                GPIO_AF_USART1
    
#define USART_ESP_TX_PIN_SOURCE        GPIO_PinSource9
#define USART_ESP_RX_PIN_SOURCE        GPIO_PinSource10
    
#define USART_ESP_RCC_GPIO             RCC_APB2Periph_GPIOA
#define USART_ESP_RCC_USART            RCC_APB2Periph_USART1
/* ================================================================================================== */

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


/* ================================================================================================== */
#define W25Q32_SPI                     SPI1
    
#define W25Q32_SPI_GPIO_PORT           GPIOA
    
#define W25Q32_CS_PIN                  GPIO_Pin_4
#define W25Q32_SCK_PIN                 GPIO_Pin_5
#define W25Q32_MISO_PIN                GPIO_Pin_6
#define W25Q32_MOSI_PIN                GPIO_Pin_7
    
#define W25Q32_SPI_GPIO_CLK            RCC_APB2Periph_GPIOA
#define W25Q32_SPI_CLK                 RCC_APB2Periph_SPI1
    
#define W25Q32_CS_LOW()                GPIO_ResetBits(W25Q32_SPI_GPIO_PORT, W25Q32_CS_PIN)
#define W25Q32_CS_HIGH()               GPIO_SetBits(W25Q32_SPI_GPIO_PORT, W25Q32_CS_PIN)
/* ================================================================================================== */
/* ===================================================================================== */


/* ====================================== API ========================================== */
void hardware_init(void);
/* ===================================================================================== */

#endif /* __HARDWARE_H__ */