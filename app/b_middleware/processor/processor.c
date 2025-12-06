#include "processor.h"

/* ====================================== GLOBAL VARIABES =========================================== */
/* ================================================================================================== */

/* ====================================== STATIC DECLARATIONS ======================================= */
/* ================================================================================================== */

void processor_init(void){
    hardware_init();
    usart_engine_init();
}

void processor(void){
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);   // LED ON
    delay_ms(500);

    GPIO_SetBits(GPIOC, GPIO_Pin_13);     // LED OFF
    delay_ms(500);
}