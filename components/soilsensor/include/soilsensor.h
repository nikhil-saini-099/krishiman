#ifndef SOILSENSOR_H
#define SOILSENSOR_H


#include <stdint.h>
#define channel_SS    4
#define atten_SS      3
#define Vdry          100 // in milli volt  
#define Vwet          2000 // in milli volt  
void soil_sensor_gpio_adc_init(void) ;                 
void soil_sensor_init();
uint16_t VWC_calc();
#endif // SOILSENSOR_H