#include "I2C.H"
#include "esp32s3.h"
#include <stdint.h>
void pin_config_I2C()
{
    IO_MUX_GPIO8_REG = (1 << IO_MUX_MCU_SEL_S) |  
          IO_MUX_FUN_IE_M        |   
          IO_MUX_FUN_WPU_M; 
    IO_MUX_GPIO9_REG =
          (1 << IO_MUX_MCU_SEL_S) |
          IO_MUX_FUN_IE_M         |
          IO_MUX_FUN_WPU_M ;
    GPIO_PIN8_REG |=  GPIO_PIN_PAD_DRIVER_M ;
    GPIO_PIN9_REG |=  GPIO_PIN_PAD_DRIVER_M ;
    GPIO_FUNC8_OUT_SEL_CFG_REG = 90 ;
    GPIO_FUNC9_OUT_SEL_CFG_REG = 89 ;
    GPIO_FUNC89_IN_SEL_CFG_REG =  9 | GPIO_SIG_IN_SEL_M ;
    GPIO_FUNC90_IN_SEL_CFG_REG =  8 | GPIO_SIG_IN_SEL_M ;
    GPIO_ENABLE_REG |=  ((1 << 8) | (1 << 9));
}
void SCL_CONFIG() {
  I2C_CLK_CONF_REG = (1 << I2C_SCLK_ACTIVE_shift) | (0 << I2C_SCLK_SEL_shift) |
                     (1 << I2C_SCLK_DIV_B_shift) | (0 << I2C_SCLK_DIV_A_shift) |
                     (0 << I2C_SCLK_DIV_NUM_shift); // DIVISOR = 1
  I2C_SCL_LOW_PERIOD_REG = I2C_SCL_LOW_PERIOD_val;
  I2C_SDA_HOLD_REG = I2C_SDA_HOLD_TIME_val;
  I2C_SCL_HIGH_PERIOD_REG = I2C_SCL_HIGH_PERIOD_val | I2C_SCL_WAIT_HIGH_PERIOD_val;
  I2C_SDA_SAMPLE_REG = I2C_SDA_SAMPLE_TIME_val;
}
void I2C_MASTER_INIT() {
 
  SYSTEM_PERIP_CLK_EN0_REG |= SYSTEM_I2C_EXT0_CLK_EN;
  I2C_CTR_REG |= I2C_CLK_EN;
  I2C_CTR_REG |= I2C_MS_MODE;
  // I2C_FIFO_CONF_REG |= I2C_NONFIFO_EN; because we chosen FIFO mode for TX and
  // rx
   SCL_CONFIG();
  I2C_CTR_REG |= I2C_CONF_UPGATE;
}
void TEMP_COMND_SET() {
  I2C_COMD0_REG = (RSTART);
  I2C_COMD1_REG = (WRITE) | 1;
  I2C_COMD2_REG = (WRITE) | 1;
  I2C_COMD3_REG = (RSTART);
  I2C_COMD4_REG = (WRITE) | 1;
  I2C_COMD5_REG = (READ) | (1 << ack_exp_en_shift) | (2);
  I2C_COMD6_REG =
      (READ) | (1 << ack_value_shift) | (1 << ack_exp_en_shift) | (1);
  I2C_COMD7_REG = (STOP);

  I2C_DATA_REG = SLAVE_ADDR;
  I2C_DATA_REG = temp_comnd;
  I2C_DATA_REG = SLAVE_ADDR;
}
void HUMID_COMND_SET() {
  I2C_COMD0_REG = (RSTART); // in esp32s3 theres no start only RStart commnd
  I2C_COMD1_REG = (WRITE) | 1;
  I2C_COMD2_REG = (WRITE) | 1;
  I2C_COMD3_REG = (RSTART);
  I2C_COMD4_REG = (WRITE) | 1;
  I2C_COMD5_REG = (READ) | (1 << ack_exp_en_shift) | (2);
  I2C_COMD6_REG =
      (READ) | (1 << ack_value_shift) | (1 << ack_exp_en_shift) | (1);
  I2C_COMD7_REG = (STOP);

  I2C_DATA_REG = SLAVE_ADDR;
  I2C_DATA_REG = humid_comnd;
  I2C_DATA_REG = SLAVE_ADDR;
}

uint16_t I2C_RX_RAW(HTU_mode MODE) {

  if (MODE == TEMP)
    TEMP_COMND_SET();
  else
    HUMID_COMND_SET();

  I2C_CTR_REG |= I2C_TRANS_START;

  uint32_t timeout = 5000000;

  while (!(I2C_INT_RAW_REG & I2C_RXFIFO_WM_INT_RAW_mask)) {
    if (--timeout == 0) {
      return 0;
    }
  }

  uint8_t msb = I2C_DATA_REG;
  uint8_t lsb = I2C_DATA_REG; // 6bit data + 2 bit crc
  // uint8_t crc = I2C_DATA_REG; can be use for error detection
  uint8_t crc = I2C_DATA_REG; 
  uint16_t raw = ((uint16_t)msb << 8) | lsb;
  raw &= 0xfffc ;
  return raw;
}