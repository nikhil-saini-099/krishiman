#ifndef FSM_H
#define FSM_H

// 1. Header files needed
#include <stdint.h>
typedef enum
{
      Dry,
      Normal,
      Floody ,
      NUM_STATES
} States ;
typedef enum 
{
      EVT_DRY    ,
        EVT_NORMAL ,
        EVT_FLOODY ,
      NUM_EVENTS
} Events ;
static const States FSM_TABLE[NUM_STATES][NUM_EVENTS] =
{
    [Dry] = {
        [EVT_DRY]    = Dry,
        [EVT_NORMAL] = Normal,
        [EVT_FLOODY] = Floody
    },

    [Normal] = {
        [EVT_DRY]    = Dry,
        [EVT_NORMAL] = Normal,
        [EVT_FLOODY] = Floody
    },

    [Floody] = {
        [EVT_DRY]    = Dry,
        [EVT_NORMAL] = Normal,
        [EVT_FLOODY] = Floody
    }
};



// soil sensor fsm intialisatio

States current_state = Dry ;
Events current_Event = EVT_DRY ;
// fsm handler 
void EVENT_checker(uint32_t vwc) ;
void SOIL_SENSOR_FSM_ENGINE(uint32_t vwc) ;


#endif // ADC_H