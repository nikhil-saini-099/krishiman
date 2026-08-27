
#include "adc.h"
#include "esp32s3.h"
#include <math.h>
#include <stdint.h>


void adc_init(void) {
  SENS_SAR_PERI_RESET_CONF_REG |= SENS_TSENS_RESET;
 SENS_SAR_PERI_CLK_GATE_CONF_REG &= ~(SENS_SARADC_CLK_EN);
  SENS_SAR_PERI_CLK_GATE_CONF_REG |= SENS_SARADC_CLK_EN;
   SENS_SAR_MEAS1_CTRL2_REG |= SENS_MEAS1_START_FORCE ;
  SENS_SAR_MEAS1_CTRL2_REG |= EN_PAD_FORCE;
  SENS_SAR_MEAS1_CTRL2_REG |= EN_PAD;
  SENS_SAR_ATTEN1_REG |= SENS_SAR1_ATTEN;
  SENS_SAR_READER1_CTRL_REG |= SENS_SAR1_CLK_DIV;
}
uint16_t adc_read(void)
{
    uint16_t data ;
    SENS_SAR_MEAS1_CTRL2_REG |= SENS_MEAS1_START_SAR ;
    
    while (!(SENS_SAR_MEAS1_CTRL2_REG & SENS_MEAS1_DONE_SAR)) ;
    data = SENS_SAR_MEAS1_CTRL2_REG ;
    data = (data & SENS_MEAS1_DATA_SAR)>>(SENS_MEAS1_DATA_SAR_BIT);
    return data ;
}
float adc_rawtovoltage(uint16_t raw)
{
    return (4400.0f * raw) / 4095.0f;
}