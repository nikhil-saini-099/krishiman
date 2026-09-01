#include "HTU21D.h"
#include "I2C.H"
#include <stdint.h>
void HTU21D_init()
{
    pin_config_I2C() ;
    I2C_MASTER_INIT() ;
}
float temp_calc()
{
    uint16_t raw = I2C_RX_RAW(TEMP) ;
    float temp =
    -46.85f + (175.72f * (float)raw / 65536.0f);      return temp ;
}
float humid_calc()
{
      uint16_t raw = I2C_RX_RAW(HUMID) ;
float humid =
    -6.0f + (125.0f * (float)raw / 65536.0f);      return humid ;
}
