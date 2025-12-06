#include "hardware.h"

/* ====================================== STATIC DECLARATIONS ======================================= */
static void led_debug_init(void);
static void Clock_72MHz_HSE_Init(void);
/* ================================================================================================== */


void USART_Debugger_Init(void)
{
    RCC_APB2PeriphClockCmd(USART_DEBUGGER_RCC_GPIO, ENABLE);
    RCC_APB1PeriphClockCmd(USART_DEBUGGER_RCC_USART, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = USART_DEBUGGER_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   
    GPIO_Init(USART_DEBUGGER_GPIO_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = USART_DEBUGGER_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART_DEBUGGER_GPIO_PORT, &GPIO_InitStructure);


    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = USART_DEBUGGER_BAUD;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART_DEBUGGER_PORT, &USART_InitStructure);
    USART_Cmd(USART_DEBUGGER_PORT, ENABLE);
}

static void led_debug_init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = GPIO_Pin_13;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &gpio);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

/**
 * @brief   72Mhz, HSE as source
 */
static void Clock_72MHz_HSE_Init(void) {
    ErrorStatus HSEStartUpStatus;

    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    
    if(HSEStartUpStatus == SUCCESS) {
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_2);
        
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK2Config(RCC_HCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        RCC_PLLCmd(ENABLE);
        
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08);
    }

    SystemCoreClockUpdate();
}

void hardware_init(void){
    Clock_72MHz_HSE_Init();
    systick_init(1000);
    USART_Debugger_Init();
}