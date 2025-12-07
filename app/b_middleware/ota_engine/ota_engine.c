#include "ota_engine.h"

/* ====================================== GLOBAL VARIABLES ========================================== */
bool ota_detect = false;                    /*<! USE TO ENABLE OTA UPDATE */
ota_state_t ota_state = OTA_IDLE_STATE;     

uint32_t ota_total_size = 0;                /*<! TOTAL REAL SIZE */
uint32_t ota_received   = 0;                /*<! USE TO COUND BYTE WHEN RECEIVE FROM ESP */

uint8_t ota_buff[512];                      /*<! BUFFER TO STORE RECEIVED DATA */
uint16_t data_idx = 0;                      /*<! COUNT DATA WHEN RECEIVING */
/* ================================================================================================== */

/* ====================================== STATIC DECLARATIONS ======================================= */
static volatile uint32_t fw_size;           /*<! VALUE FW SIZE ON RAM */
static volatile uint32_t fw_flag;           /*<! VALUE FLAG ON RAM */
/* ================================================================================================== */

void ota_init(void){
    FLASH_ErasePage(META_ADDR);
    fw_size = 0;
    fw_flag = *((uint32_t*)FW_FLAG_ADDR);

    if (fw_flag != OTA_FLAG_READY && fw_flag != OTA_FLAG_NONE)  /*<! IF FLAG IS NOT 1 IN THAT 2 VALUE */
    {
        fw_flag = OTA_FLAG_NONE;
    }
}

void set_fw_size(uint32_t size){
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_ProgramWord(FW_SIZE_ADDR, size);
    FLASH_Lock();
}

void set_fw_flag(uint32_t flag){
    if (flag != OTA_FLAG_NONE && flag != OTA_FLAG_READY)
        return;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_ProgramWord((uint32_t)FW_FLAG_ADDR, flag);
    FLASH_Lock();
}


