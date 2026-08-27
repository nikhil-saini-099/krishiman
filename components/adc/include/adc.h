#ifndef adc_H
#define adc_H

#include <stdint.h>

// SENS_SAR_PERI_RESET_CONF_REG
#define SENS_TSENS_RESET_BIT                                                30
#define SENS_TSENS_RESET                                                    (1U<< SENS_TSENS_RESET_BIT )
// clk_gate_config
#define SENS_SARADC_CLK_EN_BIT                                              30
#define SENS_SARADC_CLK_EN                                                  (1U << SENS_SARADC_CLK_EN_BIT)


// SENS_SAR_MEAS1_CTRL2_REG
#define EN_PAD_FORCE_BIT                                                    31
#define EN_PAD_FORCE                                                        (1U<<EN_PAD_FORCE_BIT)
#define EN_PAD_BIT                                                          19
#define EN_PAD                                                              (1U<<(EN_PAD_BIT + 4))// currently using channel 4
#define SENS_MEAS1_START_FORCE_BIT                                          18
#define SENS_MEAS1_START_FORCE                                              (1U<< SENS_MEAS1_START_FORCE_BIT)
#define SENS_MEAS1_START_SAR_BIT                                            17
#define SENS_MEAS1_START_SAR                                                (1U<<SENS_MEAS1_START_SAR_BIT)
#define SENS_MEAS1_DONE_SAR_BIT                                             16
#define SENS_MEAS1_DONE_SAR                                                 (1U<<SENS_MEAS1_DONE_SAR_BIT)
#define SENS_MEAS1_DATA_SAR_BIT                                             0
#define SENS_MEAS1_DATA_SAR                                                 (0XFFFFU<<0) 

// SENS_SAR_ATTEN1_REG 
#define SENS_SAR1_ATTEN                                                     (3U<< (2*4))        // 8th and 9 th bit will be use for 4th channel      

// SENS_SAR_READER1_CTRL_REG 
#define SENS_SAR1_CLK_DIV_BIT                                                0
#define SENS_SAR1_CLK_DIV                                                    (4U<<SENS_SAR1_CLK_DIV_BIT)                      

//SENS_SAR_MEAS1_MUX_REG  
#define SENS_SAR1_DIG_FORCE                                                  (1U<<31)



void adc_init();
uint16_t adc_read();
float adc_rawtovoltage(uint16_t raw) ;
#endif