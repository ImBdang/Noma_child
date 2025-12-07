#include "hardware.h"

/* ====================================== STATIC DECLARATIONS ======================================= */
static void W25Q32_SPI_Init(void);
static void ESP32_USART_Init(void);
static void led_debug_init(void);
static void Clock_72MHz_HSE_Init(void);
static void LED_Init(void);
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


/**
 * @brief   Init USART1 co communicate with ESP32
 */
void ESP32_USART_Init(void)
{
    RCC_APB2PeriphClockCmd(USART_ESP_RCC_GPIO | USART_ESP_RCC_USART, ENABLE);

    GPIO_InitTypeDef gpio_tx = {
        .GPIO_Pin   = USART_ESP_TX_PIN,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode  = GPIO_Mode_AF_PP
    };
    GPIO_Init(USART_ESP_GPIO_PORT, &gpio_tx);

    GPIO_InitTypeDef gpio_rx = {
        .GPIO_Pin   = USART_ESP_RX_PIN,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode  = GPIO_Mode_IN_FLOATING
    };
    GPIO_Init(USART_ESP_GPIO_PORT, &gpio_rx);

    USART_InitTypeDef uart_cfg = {
        .USART_BaudRate            = USART_ESP_BAUD,
        .USART_WordLength          = USART_WordLength_8b,
        .USART_StopBits            = USART_StopBits_1,
        .USART_Parity              = USART_Parity_No,
        .USART_Mode                = USART_Mode_Tx | USART_Mode_Rx,
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None
    };

    USART_Init(USART_ESP, &uart_cfg);

    USART_ITConfig(USART_ESP, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef nvic_cfg = {
        .NVIC_IRQChannel = USART1_IRQn,             
        .NVIC_IRQChannelPreemptionPriority = 0,
        .NVIC_IRQChannelSubPriority        = 0,
        .NVIC_IRQChannelCmd                = ENABLE
    };
    NVIC_Init(&nvic_cfg);

    USART_Cmd(USART_ESP, ENABLE);
}


/**
 * @brief Init GPIOA and SPI1
 * 
 * @retval None
 */
void W25Q32_SPI_Init(void)
{
    RCC_APB2PeriphClockCmd(W25Q32_SPI_GPIO_CLK | W25Q32_SPI_CLK, ENABLE);

    GPIO_InitTypeDef gpio_cs = {
        .GPIO_Pin   = W25Q32_CS_PIN,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode  = GPIO_Mode_Out_PP
    };

    GPIO_InitTypeDef gpio_sck_mosi = {
        .GPIO_Pin   = W25Q32_SCK_PIN | W25Q32_MOSI_PIN,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode  = GPIO_Mode_AF_PP
    };

    GPIO_InitTypeDef gpio_miso = {
        .GPIO_Pin   = W25Q32_MISO_PIN,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode  = GPIO_Mode_IN_FLOATING
    };

    GPIO_Init(W25Q32_SPI_GPIO_PORT, &gpio_cs);
    GPIO_Init(W25Q32_SPI_GPIO_PORT, &gpio_sck_mosi);
    GPIO_Init(W25Q32_SPI_GPIO_PORT, &gpio_miso);

    W25Q32_CS_HIGH();

    SPI_InitTypeDef spi_config = {
        .SPI_Direction         = SPI_Direction_2Lines_FullDuplex,
        .SPI_Mode              = SPI_Mode_Master,
        .SPI_DataSize          = SPI_DataSize_8b,
        .SPI_CPOL              = SPI_CPOL_Low,
        .SPI_CPHA              = SPI_CPHA_1Edge,
        .SPI_NSS               = SPI_NSS_Soft,
        .SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4,
        .SPI_FirstBit          = SPI_FirstBit_MSB,
        .SPI_CRCPolynomial     = 7
    };
    SPI_Init(W25Q32_SPI, &spi_config);
    SPI_Cmd(W25Q32_SPI, ENABLE);
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

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(LED_GPIO_Port, LED_Pin);
}


void hardware_init(void){
    SCB->VTOR = 0x08004000;
    Clock_72MHz_HSE_Init();
    systick_init(1000);
    W25Q32_SPI_Init();
    ESP32_USART_Init();
    USART_Debugger_Init();
    LED_Init();
}