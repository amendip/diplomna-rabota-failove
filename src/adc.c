#include "adc.h"

void adcinitdma(){
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  
  ADC_DMACmd(ADC1, DISABLE);
  ADC_Cmd(ADC1, DISABLE);
    
  /* Enable peripheral clocks *************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* DMA2 Stream0 channel2 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
  //DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)0x4001204C);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)&ADC1->DR);
  if(adcbuf)
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)wb2b;
  else
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)wb2;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 2*ADCBL;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);
    
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; //???
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  //ADC1->CR2 |= (1<<10);



ADC_EOCOnEachRegularChannelCmd(ADC1, DISABLE);

  /* ADC3 regular channel7 configuration **************************************/
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, nhsps);

 /* Enable DMA request after last transfer (Single-ADC mode) */
 //ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
 ADC_DMARequestAfterLastTransferCmd(ADC1, DISABLE);

  /* Enable ADC3 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC3 */
  ADC_Cmd(ADC1, ENABLE);

}

void adcch0(){
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);
}

void adcch3(){
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_15Cycles);
}

void adcch6(){
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);
}

void adcpininit(){
  GPIO_InitTypeDef      GPIO_InitStructure;
    
  /* Enable peripheral clocks *************************************************/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void adcpolling(){
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  
  ADC_DMACmd(ADC1, DISABLE);
  ADC_Cmd(ADC1, DISABLE);
  
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  //ADC1->CR2 |= (1<<10);

ADC_EOCOnEachRegularChannelCmd(ADC1, ENABLE);

  /* ADC3 regular channel7 configuration **************************************/
  adcch0();

  /* Enable ADC3 */
  ADC_Cmd(ADC1, ENABLE);

}


void adcpoll(){
uint8_t tt=1;
if(ADC_GetFlagStatus(ADC1, ADC_FLAG_OVR)){
//banner[19]++;
//if(banner[19]>'9') banner[19]='0', banner[18]++;
//if(banner[18]>'9') banner[18]='0', banner[17]++;
ADC_ClearFlag(ADC1, ADC_FLAG_OVR);
}
ADC_SoftwareStartConv(ADC1);
while(1){
if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==SET){
//banner[12]++;
//if(banner[12]>'9') banner[12]='0', banner[11]++;
//if(banner[11]>'9') banner[11]='0', banner[10]++;
break; 
}
if(tt>100){
//banner[19]++;
//if(banner[19]>'9') banner[19]='0', banner[18]++;
//if(banner[18]>'9') banner[18]='0', banner[17]++;
break;
}
tt++;
}
}


uint16_t adccnt=0;
uint16_t triglvl=160;
uint8_t adcend=0;
uint8_t adcpause=0;
uint8_t adcmode=0;
uint8_t adcbuf=0, adcbad=0, adcbbd=1;
uint8_t adccc=1;
uint32_t hsynccnt=0; 
void hsyncsample(){
//DMA2_Stream0->CR|=DMA_SxCR_EN;
uint16_t tmp=0;
/*if(adcend){
ADC_SoftwareStartConv(ADC1);
tmp=ADC1->DR;
if(tmp<((uint16_t)100)<<4 || tmp>((uint16_t)150)<<4) return;
}
*/
/* if(adcend){
if((!adcbuf) && adcbad)
adcbad=0;
else if(adcbuf && adcbbd)
adcbbd=0;
else return;
}else
*/
if(adcend || adcpause) return;
if(adccnt>=2*ADCBL){
adccnt=0;
adcbuf=!adcbuf;
adcend=1;
return;
}

//  ADC_Cmd(ADC1, DISABLE);
//  ADC_Cmd(ADC1, ENABLE);
adcch0();
adcpoll();
//while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) && tt<64) tt++;
//while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==0);
//ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC);
//  ADC_Cmd(ADC1, DISABLE);
if(adcbuf)
//lb1[adccnt][0]=(wb2b[adccnt]=ADC1->DR)>>4;
wb2b[adccnt]=ADC1->DR;
else
//lb1[adccnt][0]=(wb2[adccnt]=ADC1->DR)>>4;
wb2[adccnt]=ADC1->DR;

//lb1[adccnt][0]=ADC1->DR;

/*
if(adccnt){
//lb1[adccnt-1][1]=((uint16_t)lb1[adccnt-1][0]+(uint16_t)lb1[adccnt][0])>>1;
if(lb1[adccnt-1][0]>lb1[adccnt][0])
lb1[adccnt-1][1]=(uint16_t)lb1[adccnt][0];
else{
lb1[adccnt-1][1]=(uint16_t)lb1[adccnt-1][0];
lb1[adccnt-1][0]=(uint16_t)lb1[adccnt][0];
}
}
*/

//for(uint32_t i=300; i>0; i--);

if(adccc>1){
adcch3();
//  ADC_Cmd(ADC1, ENABLE);
//while((!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) || tt) tt--;
adcpoll();
//  ADC_Cmd(ADC1, DISABLE);
//for(int i=0;i<90;i++);
if(adcbuf)
wb3b[adccnt]=ADC1->DR;
else
wb3[adccnt]=ADC1->DR;

if(adccc>2){
adcch6();
adcpoll();
if(adcbuf)
wb4b[adccnt]=ADC1->DR;
else
wb4[adccnt]=ADC1->DR;
}

}
//if(adccnt&1)
//adcch3();
//else
//adcch0();

adccnt++;
//DMA2_Stream0->CR|=((uint32_t)DMA_SxCR_EN);
}

uint16_t adcsample(){
ADC_SoftwareStartConv(ADC1);
return ADC_GetConversionValue(ADC1);
}
