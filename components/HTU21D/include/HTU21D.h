#ifndef HTU21D_H
#define HTU21D_H
#include "esp32s3.h"
#include "I2C.H"
#define IO_MUX_MCU_SEL_S   12
#define IO_MUX_MCU_SEL_M   (0x7 << IO_MUX_MCU_SEL_S)
#define IO_MUX_FUN_IE_M    (1 << 9)
#define IO_MUX_FUN_WPU_M   (1 << 8)
#define IO_MUX_FUN_WPD_M   (1 << 7)
#define GPIO_PIN_PAD_DRIVER_M (1 << 2)
float temp_calc() ;
float humid_calc() ;
void HTU21D_init() ;

#endif
