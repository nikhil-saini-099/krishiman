/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "HTU21D.h"
#include "I2C.H"
#include "adc.h"
#include "esp32s3.h"
#include "soilsensor.h"
#include <stdint.h>
#include <stdio.h>
#define IO_MUX_GPIO4_REG        (*(volatile uint32_t *)(DR_REG_IO_MUX_BASE + 0x10))
#define GPIO_ENABLE_W1TS_REG    (*(volatile uint32_t *)(DR_REG_GPIO_BASE + 0x24)) // Write 1 to Set
/* ESP32-S3 GPIO Output Registers Base Offsets */
#define GPIO_OUT_W1TS_REG       (*(volatile uint32_t *)(DR_REG_GPIO_BASE + 0x08)) // Write 1 to Set High
#define GPIO_OUT_W1TC_REG       (*(volatile uint32_t *)(DR_REG_GPIO_BASE + 0x0C)) // Write 1 to Clear Low

/* Turn LED ON (Drive GPIO4 High) */
void led_on(void) {
    GPIO_OUT_W1TS_REG = (1 << 4);
}

/* Turn LED OFF (Drive GPIO4 Low) */
void led_off(void) {
    GPIO_OUT_W1TC_REG = (1 << 4);
}
void led_gpio_init(void) {
    /* 1. Configure IO MUX for GPIO4:
       Set MCU_SEL to standard GPIO function (Function 2, bits [14:12] = 0),
       Enable input buffer (FUN_IE = 1) if reading state is needed, 
       Disable pull-ups/pull-downs. */
    uint32_t reg_val = IO_MUX_GPIO4_REG;
    reg_val &= ~(0x7 << 12);     // Clear MCU_SEL (Function 0 / GPIO)
    reg_val |=  (1 << 9);        // Enable input buffer (FUN_IE) so read operations work if needed
    reg_val &= ~((1 << 16) | (1 << 17)); // Clear pull-up and pull-down resistors
    IO_MUX_GPIO4_REG = reg_val;

    /* 2. Configure GPIO Matrix / Digital Output Enable for GPIO4
       Writing a 1 to bit 4 of GPIO_ENABLE_W1TS_REG enables the digital output driver. */
    GPIO_ENABLE_W1TS_REG = (1 << 4);
}
void delay(uint32_t n)
{
        while(n>0)
        { n-- ;}
}
void app_main(void) {
  // SOIL SENSOR SETUP
  soil_sensor_init();
  HTU21D_init();
  led_gpio_init() ;
  while(1)
  {
        delay(30000);
        uint32_t vwc = VWC_calc() ;
        if(vwc>Vwet)led_on(); 
        else led_off() ;
  }
  
}