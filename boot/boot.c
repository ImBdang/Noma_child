#include "debugger.h"
#include "stm32f10x.h"
#include "stdint.h"
#include "stdbool.h"
#include "hardware.h"

/* ====================================== DEFINATIONS =============================================== */
#define META_ADDR               0x08014000
#define FW_SIZE_ADDR            (META_ADDR)
#define FW_FLAG_ADDR            (META_ADDR + 4)

#define OTA_FLAG_NONE           0xFFFFFFFF
#define OTA_FLAG_READY          0xA5A5A5A5

#define APP_START_ADDR          0x08004000

#define CHUNK                   256
#define FLASH_PAGE_SIZE         1024 
/* ================================================================================================== */

/* ====================================== STATIC DECLARATIONS ======================================= */
static volatile uint32_t fw_size = 0;
/* ================================================================================================== */

typedef void (*app_entry_t)(void);

void jump_to_app(void)
{
  DEBUG_PRINT("JUMP TO APP\r\n");
  uint32_t app_sp = *(volatile uint32_t*)(APP_START_ADDR);
  uint32_t app_pc = *(volatile uint32_t*)(APP_START_ADDR + 4);

  __disable_irq();
  SysTick->CTRL = 0;        
  SysTick->LOAD = 0;       
  SysTick->VAL = 0;    

  for (int i = 0; i < 8; i++) {
    NVIC->ICER[i] = 0xFFFFFFFF;  
    NVIC->ICPR[i] = 0xFFFFFFFF;  
  }

  SCB->VTOR = APP_START_ADDR;
  __set_MSP(app_sp);

  __enable_irq();

  app_entry_t app_entry = (app_entry_t)app_pc;
  app_entry();
}


void Flash_EraseAppRegion64K(void)
{
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR | FLASH_FLAG_EOP);

  for (uint32_t addr = APP_START_ADDR; 
        addr < (APP_START_ADDR + 64 * 1024);
        addr += FLASH_PAGE_SIZE)
  {
      FLASH_ErasePage(addr);
  }

  FLASH_Lock();
}


bool Flash_Write(uint32_t address, const uint8_t *data, uint32_t length)
{
    // Flash writes only 16-bit (halfword)
    if (address % 2 != 0) {
        return false;
    }

    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR | FLASH_FLAG_EOP);

    uint32_t i = 0;

    while (i < length)
    {
        uint16_t halfword;

        if (i + 1 < length)
        {
            // Normal: enough data for full halfword
            halfword = data[i] | (data[i+1] << 8);
        }
        else
        {
            // Last odd byte: pad with 0xFF
            halfword = data[i] | (0xFF << 8);
        }

        FLASH_Status status = FLASH_ProgramHalfWord(address + i, halfword);
        if (status != FLASH_COMPLETE)
        {
            FLASH_Lock();
            return false;
        }

        i += 2;
    }

    FLASH_Lock();
    return true;
}


void meta_init(void)
{
  fw_size = *((uint32_t*)FW_SIZE_ADDR);
}

bool Flash_CopyFromW25_ToApp(uint32_t fw_size)
{
  uint8_t buffer[CHUNK];
  uint32_t src_addr = 0;             
  uint32_t dst_addr = APP_START_ADDR;
  uint32_t remaining = fw_size;

  while (remaining > 0)
  {
      uint32_t chunk = (remaining > CHUNK) ? CHUNK : remaining;
      W25Q32_ReadData(src_addr, buffer, chunk);
      if (!Flash_Write(dst_addr, buffer, chunk))
      {
          return false;
      }

      src_addr += chunk;
      dst_addr += chunk;
      remaining -= chunk;
  }

  return true;
}

void clear_ota_flag(void)
{
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR | FLASH_FLAG_EOP);
    FLASH_Status st = FLASH_ErasePage(META_ADDR);
    if (st == FLASH_COMPLETE) {
        st = FLASH_ProgramWord(FW_FLAG_ADDR, OTA_FLAG_NONE);
    }

    FLASH_Lock();
}

bool check_ota(void){
  uint32_t flag = *((uint32_t*)FW_FLAG_ADDR);
  return (flag == OTA_FLAG_READY);
}



int main(void)
{
  hardware_init();
  meta_init();
  DEBUG_PRINT("Bootloader run\r\n");

  if (check_ota()){

    DEBUG_PRINT("NEW FIRMWARE DETECT\r\n");
    Flash_EraseAppRegion64K();
    Flash_CopyFromW25_ToApp(fw_size);
    clear_ota_flag();
    NVIC_SystemReset();  
  }

  jump_to_app();
  while(1){}
  return 0;
}
