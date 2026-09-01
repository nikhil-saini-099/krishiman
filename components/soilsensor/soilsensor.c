#include "adc.h"
#include <stdint.h>
#include "soilsensor.h" 
#include "esp32s3.h"
void soil_sensor_gpio_adc_init(void) {
    /* 1. Configure IO MUX for GPIO5: 
       Disable input buffer (FUN_IE), output buffer (FUN_OE), and pull-ups/pull-downs. 
       Clear bit 9 (FUN_IE), bit 16/17 (FUN_PU/FUN_PD) */
    uint32_t reg_val = IO_MUX_GPIO5_REG;
    reg_val &= ~((1 << 9)  | // Clear FUN_IE (Input Enable)
                 (1 << 16) | // Clear FUN_PU (Pull-up)
                 (1 << 17) | // Clear FUN_PD (Pull-down)
                 (1 << 13)); // Clear FUN_OE if set via MUX function
    IO_MUX_GPIO5_REG = reg_val;

    /* 2. Clear Digital GPIO Matrix Enable for GPIO5 
       Writing a 1 to bit 5 of W1TC (Write 1 to Clear) disables digital output */
    GPIO_ENABLE_W1TC_REG = (1 << 5);

    /* 3. Configure SAR ADC1 Channel 4 (GPIO5) 
       Set SAR ADC1 attenuation for Channel 4 (Bits [9:8] for CH4 in SENS_SAR1_ATTEN_REG)
       Attenuation value 3 = 11dB (approx 0V - 3.3V range) */
    uint32_t atten_val = SENS_SAR1_ATTEN_REG;
    atten_val &= ~(0x3 << (4 * 2));       /* Clear attenuation bits for CH4 (2 bits per channel) */
    atten_val |=  (0x3 << (4 * 2));       /* Set 11dB attenuation (3) for ADC1_CH4 */
    SENS_SAR1_ATTEN_REG = atten_val;

    /* Configure SAR ADC Bit Width (12-bit resolution) 
       SENS_SAR1_BIT_WIDTH field in SENS_SAR_READ_CTRL_REG (typically bits [4:3] or similar) */
    SENS_SAR_READ_CTRL_REG |= (3 << 3);   /* 3 corresponds to 12-bit width */

    /* Enable Software Force Control Mode for ADC Start if using direct register triggering */
    SENS_SAR_START_FORCE_REG |= (1 << 23); /* Enable SAR ADC controller power/force state */
}
void soil_sensor_init(){
      soil_sensor_gpio_adc_init();
      adc1_init();
      adc1_CHX_init(channel_SS , atten_SS) ;
     
}
uint16_t VWC_calc()
{     
      uint16_t Vcurrent = adc_rawtovol(adc1_read()) ;
      uint16_t vwc = (Vdry - Vcurrent)/(Vdry - Vwet) ;
      return vwc ;   
}