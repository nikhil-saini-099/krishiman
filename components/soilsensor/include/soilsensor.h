#ifndef SOILSENSOR_H
#define SOILSENSOR_H


#include <stdint.h>
#define Vdry        100 // in milli volt  
#define Vwet        2000 // in milli volt                   
uint16_t soil_sensor_init();
uint16_t VWC_calc(uint16_t Vcurrent) ;
#endif // SOILSENSOR_H