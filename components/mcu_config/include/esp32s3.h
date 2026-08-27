#ifndef ESP32S3_H
#define ESP32S3_H




#define __vt                                                            volatile 
#define LOW_POWER_MANAGEMENT                                            (0x60008000U)
#define SENS_SAR_PERI_CLK_GATE_CONF_REG                                *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT + 0x0104U)
#define SENS_SAR_PERI_RESET_CONF_REG                                   *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT + 0x0108U)
#define SENS_SAR_MEAS1_CTRL2_REG                                       *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT + 0x000cU)
#define SENS_SAR_ATTEN1_REG                                            *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT + 0x0014U)
#define SENS_SAR_READER1_CTRL_REG                                      *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT + 0x0000U) 
#define SENS_SAR_MEAS1_MUX_REG                                         *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT  + 0x0010U)
#endif // ESP32S3_H