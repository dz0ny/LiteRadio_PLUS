#ifndef __CRSF_H_
#define __CRSF_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#define DEFAULT_VALUE  993

void CRSF_SetBind(void);
void CRSF_Init(uint8_t protocolIndex);
uint16_t CRSF_Process(uint16_t* controlData);
void Get_CRSFPackage(uint8_t* ChannelToCRSF,uint16_t* ControlDataBuff);
#endif