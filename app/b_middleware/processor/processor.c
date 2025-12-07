#include "processor.h"

/* ====================================== GLOBAL VARIABES =========================================== */
/* ================================================================================================== */

/* ====================================== STATIC DECLARATIONS ======================================= */
/* ================================================================================================== */

void W25Q32_DumpFirst256(void) {
    uint8_t buf[256];

    W25Q32_ReadData(0x000000, buf, 256);

    DEBUG_PRINT("===== W25Q32 FIRST 256 BYTES =====\r\n");

    for (int i = 0; i < 256; i++) {
        DEBUG_PRINT("%02X ", buf[i]);
        if ((i + 1) % 16 == 0) {
            DEBUG_PRINT("\r\n");
        }
    }

    DEBUG_PRINT("===== END DUMP =====\r\n");
}


void processor_init(void){
    hardware_init();
    usart_engine_init();
    ota_init();
    DEBUG_PRINT("PROCESSOR INIT\r\n");
}

void processor(void){
    esp32_parse();      /*<! Hanlde command from ESP first */
    GPIO_ResetBits(LED_GPIO_Port, LED_Pin);
    delay_ms(250);
    GPIO_SetBits(LED_GPIO_Port, LED_Pin);
    delay_ms(250);
}