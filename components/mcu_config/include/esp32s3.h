#ifndef ESP32S3_H
#define ESP32S3_H




// #include <stdint.h>
#define __vt                                                            volatile
#include  <stdint.h>
 #define SYSTEM_PERIP_CLK_EN0_REG                                      *( __vt uint32_t  *)(0x600C0000U + (0x0018U))
 // base addresses 
 #define DR_REG_IO_MUX_BASE 0x60009000U
 #define DR_REG_GPIO_BASE   0x60004000U
 #define DR_REG_SENS_BASE   0x60008800U
 #define IO_MUX_GPIO8_REG  *( __vt uint32_t  *)(DR_REG_IO_MUX_BASE + 0x2CU)   // 0x6000902C
 #define IO_MUX_GPIO9_REG  *( __vt uint32_t  *)(DR_REG_IO_MUX_BASE + 0x30U) 
 #define GPIO_PIN8_REG  *( __vt uint32_t  *)(DR_REG_GPIO_BASE + 0x88U + 4*8) 
 #define GPIO_PIN9_REG  *( __vt uint32_t  *)(DR_REG_GPIO_BASE + 0x88U + 4*9) 
 #define GPIO_FUNC8_OUT_SEL_CFG_REG *( __vt uint32_t  *)(0x60004000 + 0x554 + 4*8)
#define GPIO_FUNC9_OUT_SEL_CFG_REG  *( __vt uint32_t  *)(0x60004000 + 0x554 + 4*9)
#define GPIO_FUNC89_IN_SEL_CFG_REG *( __vt uint32_t  *)(0x60004000 + 0x154 + 4*89)
#define GPIO_FUNC90_IN_SEL_CFG_REG *( __vt uint32_t  *)(0x60004000 + 0x154 + 4*90)
#define GPIO_ENABLE_REG  *( __vt uint32_t  *)(0x60004020U)

#define IO_MUX_GPIO5_REG        (*(volatile uint32_t *)(DR_REG_IO_MUX_BASE + 0x14))
#define GPIO_ENABLE_W1TC_REG    (*(volatile uint32_t *)(DR_REG_GPIO_BASE + 0x24))

#define SENS_SAR_READ_CTRL_REG  (*(volatile uint32_t *)(DR_REG_SENS_BASE + 0x000))
#define SENS_SAR_START_FORCE_REG (*(volatile uint32_t *)(DR_REG_SENS_BASE + 0x01C))
#define SENS_SAR1_ATTEN_REG     (*(volatile uint32_t *)(DR_REG_SENS_BASE + 0x004))
// registers belong to adc setup 
#define LOW_POWER_MANAGEMENT                                            (0x60008000U)
#define SENS_SAR_PERI_CLK_GATE_CONF_REG                                *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT + 0x0104U)
#define SENS_SAR_PERI_RESET_CONF_REG                                   *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT + 0x0108U)
#define SENS_SAR_MEAS1_CTRL2_REG                                       *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT + 0x000cU)
#define SENS_SAR_ATTEN1_REG                                            *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT + 0x0014U)
#define SENS_SAR_READER1_CTRL_REG                                      *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT + 0x0000U) 
#define SENS_SAR_MEAS1_MUX_REG                                         *( __vt uint32_t  *)( LOW_POWER_MANAGEMENT  + 0x0010U)
// base addresses of I2C peripheral 
#define I2C0                                                           0x60013000U
#define I2C_1                                                          0x60027000U
                              // I2c registers 
// configuration registers 
#define  I2C_CTR_REG                                               *(volatile uint32_t *)(I2C0+(0x0004U))
#define  I2C_SLAVE_ADDR_REG                                        *(volatile uint32_t *)(I2C0+(0x0010U))
#define  I2C_TO_REG                                                *(volatile uint32_t *)(I2C0+(0x000CU))
#define  I2C_FIFO_CONF_REG                                         *(volatile uint32_t *)(I2C0+(0x0018U))
#define  I2C_FILTER_CFG_REG                                        *(volatile uint32_t *)(I2C0+(0x0050U))
#define  I2C_CLK_CONF_REG                                          *(volatile uint32_t *)(I2C0+(0x0054U))
#define  I2C_SCL_SP_CONF_REG                                       *(volatile uint32_t *)(I2C0+(0x0080U))
#define  I2C_SCL_STRETCH_CONF_REG                                  *(volatile uint32_t *)(I2C0+(0x0084U))
// I2C TIMING REGISTER 
#define I2C_SCL_LOW_PERIOD_REG                                     *(volatile uint32_t *)(I2C0+(0x0000))
#define I2C_SDA_HOLD_REG                                           *(volatile uint32_t *)(I2C0+(0x0030))
#define I2C_SCL_HIGH_PERIOD_REG                                    *(volatile uint32_t *)(I2C0+(0x0038))
#define I2C_SDA_SAMPLE_REG                                         *(volatile uint32_t *)(I2C0+(0x0034))
// status registers 
#define  I2C_SR_REG                                                *(volatile uint32_t *)(I2C0+(0x0008)) // Describes I2C work status
#define  I2C_FIFO_ST_REG                                           *(volatile uint32_t *)(I2C0+(0x0014)) // FIFO status register 
#define I2C_DATA_REG                                               *(volatile uint32_t *)(I2C0+(0x001C)) //This field is used to read data from RX FIFO, or write data to TX FIFO. (R/W
// COMMAND REGISTER 
#define I2C_COMD0_REG                                              *(volatile uint32_t *)(I2C0 +(0x0058))
#define I2C_COMD1_REG                                              *(volatile uint32_t *)(I2C0 +(0x005C))
#define I2C_COMD2_REG                                              *(volatile uint32_t *)(I2C0 +(0x0060))
#define I2C_COMD3_REG                                              *(volatile uint32_t *)(I2C0 +(0x0064))
#define I2C_COMD4_REG                                              *(volatile uint32_t *)(I2C0 +(0x0068))
#define I2C_COMD5_REG                                              *(volatile uint32_t *)(I2C0 +(0x006c))
#define I2C_COMD6_REG                                              *(volatile uint32_t *)(I2C0 +(0x0070))
#define I2C_COMD7_REG                                              *(volatile uint32_t *)(I2C0 +(0x0074))
// INTRRUPT REGISTERS 
#define I2C_INT_RAW_REG                                             *(volatile uint32_t *)(I2C0 +(0x0020))

#endif // ESP32S3_H