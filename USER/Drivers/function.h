#ifndef __FUNCTION_H
#define __FUNCTION_H
#include "stm32f1xx_hal.h"
#include "gimbal.h"
#include "switches.h"

union ChipID{
	uint32_t ChipUniqueID[3];
	uint8_t IDbyte[12];
};

void Get_ChipID(union ChipID *chipID);
uint16_t GetUniqueID(void);
void Get_CRSFUniqueID(uint8_t *masterUID);
void GetSbusPackage(uint8_t* ChannelToSbus);
uint8_t crc8(const uint8_t * ptr, uint32_t len);

uint16_t map(double Oxy, double Omin, double Omax, double Nmin, double Nmax);
#ifdef LiteRadio_Plus_CC2500
//定义各通道名称(最大支持8通道)
typedef enum
{ 
	AUX1  		= 4 , 
	AUX2		= 5 , 
	AUX3		= 6 , 
	AUX4		= 7 , 
	AUX5  		= 8 , 
	AUX6		= 9 , 
	AUX7		= 10 , 
	AUX8		= 11 , 
	AUX9  		= 12 , 
	AUX10		= 13 , 
	AUX11		= 14 , 
	AUX12		= 15 , 
}ChannelTypeDef ;
#endif

#endif
