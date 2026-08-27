
#include <stdint.h>
#include "FSM.h"
#include "soilsensor.h"
void EVENT_checker(uint32_t vwc )
{
      current_Event = (vwc < Vdry) ? EVT_DRY :
                (vwc > Vwet) ? EVT_FLOODY :
                               EVT_NORMAL;
}
void SOIL_SENSOR_FSM_ENGINE(uint32_t vwc)
{     
      EVENT_checker(vwc) ;
      current_state = FSM_TABLE[current_state][current_Event] ;
}