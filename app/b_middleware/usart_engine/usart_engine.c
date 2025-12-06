#include "usart_engine.h"

/* ====================================== GLOBAL VARIABES =========================================== */
lwrb_t esp_usart_rb;
/* ================================================================================================== */

/* ====================================== STATIC DECLARATIONS ======================================= */
static uint8_t lwrb_buffer[LWRB_BUFFER_SIZE]; 
static uint8_t line_buff[LINE_BUFFER_SIZE];

static void esp32_line_process(const char* esp32_line);
/* ================================================================================================== */

void esp32_parse(void){
    static uint16_t line_len = 0;
    uint8_t c;
    while (lwrb_read(&esp_usart_rb, &c, 1)){

        if (line_len < sizeof(line_buff) - 1){
            if (c != 0xFF){
                line_buff[line_len++] = c;
            }
        }

        if (c == '\n'){
            line_buff[line_len] = '\0';
            esp32_line_process(line_buff);
            line_len = 0;
        }
    }
}

static void esp32_line_process(const char* esp32_line){
    if (strstr(esp32_line, "OTA_ON")){
        DEBUG_PRINT("OTA DETECT\r\n");
        return;
    }
    if (strstr(esp32_line, "ESP32")){
        DEBUG_PRINT("ESP32 hello\r\n");
        return;
    }
}



void usart_sendstring(USART_TypeDef *USARTx, const char *str) {
    while (*str) {
        USART_SendData(USARTx, *str++);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}

void usart_engine_init(void){
    lwrb_init(&esp_usart_rb, lwrb_buffer, sizeof(lwrb_buffer));
}
