#ifndef _TRUERANDOM_H_
#define _TRUERANDOM_H_

#include "stm32f0xx_adc.h"
#include "stm32f0xx_crc.h"
#include "stm32f0xx_rcc.h"

static uint32_t miState;


uint32_t getTrueRandomNumber(uint16_t max) {

ADC_InitTypeDef ADC_InitStructure;

//enable ADC1 clock
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

// Initialize ADC 14MHz RC
RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
RCC_HSI14Cmd(ENABLE);
while (!RCC_GetFlagStatus(RCC_FLAG_HSI14RDY))
    ;

ADC_DeInit(ADC1);
ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO; //default
ADC_Init(ADC1, &ADC_InitStructure);

//enable internal channel
ADC_TempSensorCmd(ENABLE);

// Enable ADCperipheral
ADC_Cmd(ADC1, ENABLE);
while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET)
    ;

ADC1->CHSELR = 0; //no channel selected
//Convert the ADC1 temperature sensor, user shortest sample time to generate most noise
ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor, ADC_SampleTime_1_5Cycles);

// Enable CRC clock
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

uint8_t i;
for (i = 0; i < 8; i++) {
    //Start ADC1 Software Conversion
    ADC_StartOfConversion(ADC1);
    //wait for conversion complete
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
    }

    CRC_CalcCRC(ADC_GetConversionValue(ADC1));
    //clear EOC flag
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
}

//disable ADC1 to save power
ADC_Cmd(ADC1, DISABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);

return CRC_CalcCRC(0xBADA55E5) % max;

}


#endif
