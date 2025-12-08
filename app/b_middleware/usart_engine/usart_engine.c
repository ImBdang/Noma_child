#include "usart_engine.h"

/* ====================================== GLOBAL VARIABES =========================================== */
lwrb_t esp_usart_rb;

extern ota_state_t ota_state;
extern volatile uint32_t cur_w25_addr;
extern uint32_t ota_total_size;
extern uint32_t ota_received;
extern uint8_t ota_buff[512];
extern uint16_t data_idx;
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
        if (ota_state == OTA_READING_STATE) {
            ota_buff[data_idx++] = c;
            ota_received++;

            if (data_idx == sizeof(ota_buff) || ota_received == ota_total_size) {
                DEBUG_PRINT("Writing %d bytes to 0x%06lX\r\n", data_idx, cur_w25_addr);
                W25Q32_WriteData(cur_w25_addr, ota_buff, data_idx);
                DEBUG_PRINT("Write done\r\n");
                cur_w25_addr += data_idx;
                data_idx = 0;
            }
            if (ota_received >= ota_total_size) {
                ota_state = OTA_DONE_STATE;
            }
            continue;
        }

        if (line_len < sizeof(line_buff) - 1){
            if (c != 0xFF)
                line_buff[line_len++] = c;
        }

        if (c == '\n'){
            line_buff[line_len] = '\0';
            esp32_line_process(line_buff);
            line_len = 0;
        }
    }
}

static void esp32_line_process(const char* esp32_line){

    if (strstr(esp32_line, "OTA_BEGIN")){
        DEBUG_PRINT("%s", esp32_line);
        ota_state = OTA_WAIT_SIZE_STATE;
        ota_total_size = 0;
        ota_received = 0;
        cur_w25_addr = 0x000000;
        W25Q32_EraseChip();
        usart_sendstring(USART_ESP, "OTA_BEGIN_ACK\n");
        return;
    }

    if (strstr(esp32_line, "OTA_SIZE:")){
        ota_total_size = parse_ota_size(esp32_line);
        if (ota_total_size == 0){
            return;             /*<! DONT RESPONE, ESP32 WILL TIMEOUT */
        }
        set_fw_size(ota_total_size);
        DEBUG_PRINT("\r\nSIZE IS: %lu\r\n", ota_total_size);
        data_idx = 0;
        ota_received = 0;
        usart_sendstring(USART_ESP, "OTA_SIZE_ACK\n");
        ota_state = OTA_READING_STATE;
        return;
    }

    if (strstr(esp32_line, "OTA_END")){
        ota_state = OTA_DONE_STATE;
        set_fw_flag(OTA_FLAG_READY);
        NVIC_SystemReset(); 
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
