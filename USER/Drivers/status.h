#ifndef __STATUS_H_
#define __STATUS_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#if defined(LiteRadio_Plus_CC2500) 
#define PROTOCOL_INDEX_LIMIT 4
#define CRSF_PROTOCOL_INDEX 4
#elif defined(LiteRadio_Plus_SX1280)
#define PROTOCOL_INDEX_LIMIT 1
#define CRSF_PROTOCOL_INDEX 0
#elif defined(LiteRadio_Plus_SX1276)
#define PROTOCOL_INDEX_LIMIT 1
#define CRSF_PROTOCOL_INDEX 0
#endif
typedef enum
{
    RC_INIT              = 0,
    RC_SHUTDOWN             ,
    RC_RADIOLINK 	        ,    
    RC_CHRG_AND_JOYSTICK    ,
}RCstatus_e;

typedef enum
{
    RF_DATA           = 0,
    RF_CALIBARATION      ,   
    RF_BIND              ,
}RFstatus_e;

extern uint8_t configerRequest;
uint32_t Get_ProtocolDelayTime(void);
void Status_Init(void);
void Status_Update(void);
void statusTask(void* param);

#endif  
