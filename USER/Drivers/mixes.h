#ifndef _MIXES_H_
#define _MIXES_H_

#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "gimbal.h"
//RUDDER   = 0 ,       //yaw
//THROTTLE = 1 ,       //throttle
//AILERON  = 2 ,       //roll
//ELEVATOR = 3 ,       //pitch
typedef enum
{
  	mix_RUDDER  	= 0 ,       
	mix_THROTTLE 	= 1 ,      
	mix_AILERON 	= 2 ,      
	mix_ELEVATOR   	= 3 ,      
    mix_SWA         = 4 ,       //2POS
    mix_SWB         = 5 ,       //3POS
    mix_SWC         = 6 ,       //3POS
    mix_SWD         = 7 ,       //2POS
}mixsetChannel_e;

typedef struct
{
    mixsetChannel_e GimbalChannel;
    uint8_t mix_inverse;
    int8_t mix_weight; // 范围-100~100
    uint16_t mix_output_data;
    
}mixdata_t;


void mixesTask(void* param);

extern TaskHandle_t mixesTaskHandle;

#endif
