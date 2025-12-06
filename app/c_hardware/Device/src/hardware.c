#include "hardware.h"

/* ====================================== STATIC DECLARATIONS ======================================= */
static void W25Q32_SPI_Init(void);
static void ESP32_USART_Init(void);
/* ================================================================================================== */

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



void hardware_init(void){
    W25Q32_SPI_Init();
}