#include "joystick.h"
#include "gimbal.h"
#include "switches.h"
#include "usbd_customhid.h"
#include "mixes.h"
#include "stmflash.h"
#include "radiolink.h"
#include "function.h"
#include "status.h"
#include "crsf.h"
#include "common.h"

static uint32_t joystickDelayTime;
TaskHandle_t joystickTaskHandle;
/*累加和校验算法*/
static uint16_t checkSum;
uint8_t sendSpam;

void joystickTask(void *param) 
{
    uint16_t hidReportData[8];
    uint16_t requestDataBuff[8];
    uint16_t mixValBuff[8];

    joystickDelayTime = Get_ProtocolDelayTime();
    while(1)
    {
        vTaskDelay(joystickDelayTime);
        xQueueReceive(mixesValQueue,mixValBuff,0);
     
        hidReportData[0] = map(mixValBuff[0],1000,2000,0,2047);
        hidReportData[1] = map(mixValBuff[1],1000,2000,0,2047);
        hidReportData[2] = map(mixValBuff[2],1000,2000,0,2047);
        hidReportData[3] = map(mixValBuff[3],1000,2000,0,2047);
        hidReportData[4] = map(mixValBuff[4],1000,2000,0,2047);
        hidReportData[5] = map(mixValBuff[5],1000,2000,0,2047);
        hidReportData[6] = map(mixValBuff[6],1000,2000,0,2047);
        hidReportData[7] = map(mixValBuff[7],1000,2000,0,2047);

        if (requestType1 == REQUEST_CHANNEL_INFO)
        {
            hidReportData[0] = CHANNEILS_INFO_ID|((requestType2- 0x01) << 8);
            hidReportData[1] = mixData[requestType2- 0x01].gimbalChannel|(mixData[requestType2- 0x01].reverse << 8);
            hidReportData[2] = mixData[requestType2- 0x01].weight|(mixData[requestType2- 0x01].offset << 8);
            for(int i=0;i<7;i++)
            {
                checkSum += hidReportData[i]&0x00FF;
            }
            hidReportData[7] = checkSum; 
        }
        else if (requestType1 == REQUEST_CONIFG_INFO)
        {
            if(requestType2 == 0x00)/*lite_info*/
            {
                STMFLASH_Read(CONFIGER_INFO_ADDR,&requestDataBuff[0],3);
                hidReportData[0] = LITE_CONFIGER_INFO_ID|(VERSION_INDEX << 8);
                hidReportData[1] = requestDataBuff[0]|(requestDataBuff[1] << 8);
                hidReportData[2] = requestDataBuff[2];
                for(int i=0;i<7;i++)
                {
                    checkSum += hidReportData[i]&0x00FF;
                }
                hidReportData[7] = checkSum;    
            }
            else if(requestType2 == 0x01)/*internal_info*/
            {
                internalCRSFdata.configStatus = CONFIG_CRSF_ON;
#if defined(LiteRadio_Plus_SX1280)
                internalCRSFdata.crsfParameter.power = tx_config.power;
                internalCRSFdata.crsfParameter.rate = tx_config.rate;
                internalCRSFdata.crsfParameter.TLM = tx_config.tlm;
                hidReportData[0] = INTERNAL_CONFIGER_INFO_ID|(0x01 <<8);
                hidReportData[1] = internalCRSFdata.crsfParameter.power|(internalCRSFdata.crsfParameter.rate << 8);
                hidReportData[2] = internalCRSFdata.crsfParameter.TLM;
#endif
                for(int i=0;i<7;i++)
                {
                    checkSum += hidReportData[i] & 0x00FF;
                }
                hidReportData[7] = checkSum;

            }
            else if(requestType2 == 0x02)/*external_info*/
            {
                if(sendSpam>100)
                {
                    requestType1 = 0x00;
                    requestType2 = 0x01;
                    sendSpam = 0;
                }
                else
                {
                    sendSpam++;
                }
                hidReportData[0] = EXTERNAL_CONFIGER_INFO_ID|(0x01 <<8);;
                uint8_t currentrate;
                switch (externalCRSFdata.regulatoryDomainIndex)
                {
                    case FREQ_FCC_915:
                    case FREQ_EU_868:
                    {
                        switch (externalCRSFdata.crsfParameter.rate)
                        {
                            case RATE_200HZ:
                                currentrate = FREQ_900_RATE_200HZ;
                                break;
                            case RATE_100HZ:
                                currentrate = FREQ_900_RATE_100HZ;
                                break;
                            case RATE_50HZ:
                                currentrate = FREQ_900_RATE_50HZ;
                                break;
                            case RATE_25HZ:
                                currentrate = FREQ_900_RATE_25HZ;
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    case FREQ_ISM_2400:
                    {
                        switch (externalCRSFdata.crsfParameter.rate)
                        {
                            case RATE_500HZ:
                                currentrate = FREQ_2400_RATE_500HZ;
                                break;
                            case RATE_250HZ:
                                currentrate = FREQ_2400_RATE_250HZ;
                                break;
                            case RATE_150HZ:
                                currentrate = FREQ_2400_RATE_150HZ;
                                break;
                            case RATE_50HZ:
                                currentrate = FREQ_2400_RATE_50HZ;
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    default:
                        break;
                                
                }
                hidReportData[1] = externalCRSFdata.crsfParameter.power|(currentrate << 8);
                hidReportData[2] = externalCRSFdata.crsfParameter.TLM|(externalCRSFdata.regulatoryDomainIndex << 8);
                for(int i=0;i<7;i++)
                {
                    checkSum += hidReportData[i]&0x00FF;
                }
                hidReportData[7] = checkSum;
            }
        }
        checkSum = 0;        

        USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &hidReportData, 8*sizeof(uint16_t));
    }
}

