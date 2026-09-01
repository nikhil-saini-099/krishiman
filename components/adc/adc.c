
#include "adc.h"
#include "esp32s3.h"
#include <stdint.h>


void adc1_init(void) {
  // SYSTEM_PERIP_CLK_EN0_REG   |= SYSTEM_APB_SARADC_CLK_EN  ; // rac clk is used not dig 
  SENS_SAR_PERI_RESET_CONF_REG |= SENS_TSENS_RESET;
  SENS_SAR_PERI_CLK_GATE_CONF_REG &= ~(SENS_SARADC_CLK_EN);
  SENS_SAR_PERI_CLK_GATE_CONF_REG |= SENS_SARADC_CLK_EN;
  SENS_SAR_MEAS1_CTRL2_REG |= SENS_MEAS1_START_FORCE ;
  
  SENS_SAR_READER1_CTRL_REG |= SENS_SAR1_CLK_DIV;
  SENS_SAR_ATTEN1_REG &= ~(0xFFFF);
}
void adc1_CHX_init(uint8_t ch, uint8_t K)
{
  SENS_SAR_MEAS1_CTRL2_REG |= EN_PAD_FORCE;
  SENS_SAR_MEAS1_CTRL2_REG |= SENS_MEAS1_START_FORCE ;
  SENS_SAR_MEAS1_CTRL2_REG |= EN_PAD_FORCE;
  SENS_SAR_MEAS1_CTRL2_REG |= (1U<< (EN_PAD_BIT + ch));
  SENS_SAR_ATTEN1_REG &= ~(0xFFFF);
  SENS_SAR_ATTEN1_REG |=  (K<< (2*ch));


}
uint16_t adc1_read(void)
{
    uint16_t data ;
    SENS_SAR_MEAS1_CTRL2_REG |= SENS_MEAS1_START_SAR ;
    
    while (!(SENS_SAR_MEAS1_CTRL2_REG & SENS_MEAS1_DONE_SAR)) ;
    data = SENS_SAR_MEAS1_CTRL2_REG ;
    data = (data & SENS_MEAS1_DATA_SAR)>>(SENS_MEAS1_DATA_SAR_BIT);
    return data ;
}
float adc_rawtovol(uint16_t raw)
{
    return (4400.0f * raw) / 4095.0f;
}