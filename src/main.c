#include "main.h"
#include "stm32_ub_vga_screen.h"
#include "adc.h"

void vblank(void);

uint8_t tu=1;
int main(void)
{
  uint32_t n;

  SystemInit();

  UB_VGA_Screen_Init();

  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //wb2iala();
  adcpininit();
  adcpolling();

  //hsyncsample();

  //ADC_SoftwareStartConv(ADCx);
  uint16_t wb2ind=0, avrg=0, curval=0, preval=0, vblankcomplete=0;
  uint16_t bp=0;
  uint32_t haha=1;
  while(1)
  {
  if(invblank){
  	if(!vblankcomplete){
	vblank();
	vblankcomplete=1;
	}
  }else vblankcomplete=0;
  
  if(!(haha--)){
    curval++;
    banner[4]='0'+curval%10;
    tu=1;
    haha=1000000;
  }

  if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)){
   if(!bp){
   //banner[3]++;
   bp=60000;
   if(nhsps<200) nhsps++; else nhsps=1;
   mode=!mode;
   }
  }else if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)){
   if(!bp){
   //banner[3]++;
   bp=60000;
   if(nhsps>0) nhsps--; else nhsps=200;
   }
  }else if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)){
   if(!bp){
   bp=60000;
   if(adccc>1) adccc--; else adccc=3;
   }
  }else if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)){
   if(!bp){
   bp=60000;
   if(adccc<3) adccc++; else adccc=1;
   }
  }else if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)){
   if(!bp){
   bp=60000;
   adcpause=!adcpause;
   }
  }else{
   if(bp) bp--;
  }
 /* curval=adcsample();
  if(wb2ind<160){
  wb2[wb2ind]=curval;
  avrg=(avrg+curval)>>1;
  wb2ind++;
  }else{
  avrg=(avrg+curval)>>1;
  if(curval>=preval+64 && curval>400 && curval<600)
  wb2ind=0;
  }
  */
  //preval=curval;
  }
}

void swp(uint16_t a, uint16_t b){
	uint16_t tmp;
	tmp=a;
	a=b;
	b=tmp;
}

void vblank(){
	uint16_t fontindb, fontindn;
	char bch[41];
	if(tu){
	memcpy(bch, banner, 41);
	for(uint8_t line=0;line<16;line++){
		fontindb=(line&15)*95-0x20;
		for(uint8_t i=0;i<41;i++){
			if(!bch[i]) break;
			fontindn=fontindb+bch[i];
			for(uint8_t x=0;x<8;x++){
			if((font[fontindn]>>x)&1)
			textbuf[line][(i<<3)+x]=0xf0;
			else
			textbuf[line][(i<<3)+x]=0x00;
			}
		}
	}
	tu=0;
	}

	adcpolling();
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_28Cycles);
	adcpoll();
	nhsps=(((ADC1->DR)>>5)+2);


	if(adcend && !adcpause){
	if(adcbufbuf) adcbad=1;
	else adcbbd=1;
	//trigp=ADCBL;
	uint16_t min=65535, max=0;
	uint16_t *b2, *b3, *b4;
	if(adcbuf) b2=wb2, b3=wb3, b4=wb4; else b2=wb2b, b3=wb3b, b4=wb4b;
	for(uint16_t i=0; i<2*ADCBL; i++){
		if(b2[i]<min) min=b2[i];
		if(b2[i]>max) max=b2[i];
	}
	uint16_t triglvl;
	uint16_t maxi;
	
	triglvl=((uint32_t)min+(uint32_t)max)/(uint32_t)4;
	//triglvl=0x1FF;
	for(uint16_t i=0; i<ADCBL; i++){
		if(b2[i]<=triglvl && b2[i+1]>triglvl){
			trigp=i;
			break;
		}
	}
	if(trigp==0) banner[0]++;
	
	switch(mode){
	case 0:
	for(uint16_t i=0; i<2*ADCBL; i++){
	//banner[0]='F';
		b2[i]=300-(b2[i]>>4);
		b3[i]=300-(b3[i]>>4);
		b4[i]=300-(b4[i]>>4);
	}
	//for(uint16_t i=70; i<2*ADCBL; i+=1){
	//	b2[i]=340;
	//}
	//trigp=0;
	break;
	case 1:
	for(uint16_t i=trigp; i<ADCBL; i++){
	maxi=i;
	for(uint16_t j=i+1; j<ADCBL; j++)
	if(b2[j]>b2[maxi])
	maxi=j;


	swp(b2[i],b2[maxi]);
	swp(b3[i],b3[maxi]);
	swp(b4[i],b4[maxi]);

	b2[i]=300-(b2[i]>>4);
	b3[i]=150-(b3[i]>>5);
	//b4[i]=((b4[i]>>4)|0x3);
	b4[i]=0xFF;
	}
	break;
	}
	adcbufbuf=adcbuf;
	adcend=0;
	}
}
