#include "adc.h"
#include "stdint.h"
#include <stdint.h>
#include "soilsensor.h" 
uint16_t soil_sensor_init(){
      adc_init();
      uint16_t raw   = adc_read();
      uint16_t Vcurrent = adc_rawtovoltage(raw) ;
      return Vcurrent ; // in milli volt
}
uint16_t VWC_calc(uint16_t Vcurrent)
{     
      soil_sensor_init() ;
      uint16_t vwc = (Vdry - Vcurrent)/(Vdry - Vwet) ;
      return vwc ;   
}