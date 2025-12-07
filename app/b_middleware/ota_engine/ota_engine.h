#ifndef __OTA_H__
#define __OTA_H__

/* ====================================== INCLUDE HEADER ======================================= */
#include "stdint.h"
#include "stm32f10x.h"
#include "debugger.h"
#include "lwrb.h"
#include "stdbool.h"
#include "systick.h"
#include "spi_engine.h"
/* ============================================================================================= */


/* ====================================== DEFINITIONS ========================================== */
#define META_ADDR        0x08014000
#define FW_SIZE_ADDR    (META_ADDR)
#define FW_FLAG_ADDR    (META_ADDR + 4)

#define OTA_FLAG_NONE           0xFFFFFFFF
#define OTA_FLAG_READY          0xA5A5A5A5
/* ============================================================================================= */


/* ====================================== TYPEDEF STRUCT ======================================= */
typedef enum {
    OTA_IDLE_STATE,
    OTA_WAIT_SIZE_STATE,
    OTA_READING_STATE,
    OTA_DONE_STATE
} ota_state_t;
/* ================================================================================================== */


/* ========================================= API =============================================== */
void ota_init(void);
void set_fw_size(uint32_t size);
void set_fw_flag(uint32_t flag);
/* ============================================================================================= */

#endif /* __OTA_H__ */