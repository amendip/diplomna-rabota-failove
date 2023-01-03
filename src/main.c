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
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //wb2iala();
  adcinit();

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

  if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)){
   if(!bp){
   //banner[3]++;
   bp=60000;
   if(nhsps<200) nhsps++; else nhsps=0;
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

void vblank(){
	uint16_t fontindb, fontindn;
	if(tu){
	for(uint8_t line=0;line<16;line++){
		fontindb=(line&15)*95-0x20;
		for(uint8_t i=0;i<41;i++){
			if(!banner[i]) break;
			fontindn=fontindb+banner[i];
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

	if(adcbufbuf) adcbad=1;
	else adcbbd=1;
	adcbufbuf=adcbuf;
	//trigp=0;
	uint16_t min=65535, max=0;
	uint16_t *b2;
	if(adcbufbuf) b2=wb2; else b2=wb2b;
	for(uint16_t i=0; i<2*ADCBL; i++){
		if(b2[i]<min) min=b2[i];
		if(b2[i]>max) max=b2[i];
	}
	uint16_t triglvl;
	triglvl=((uint32_t)min+(uint32_t)max)/(uint32_t)4;
	for(uint16_t i=0; i<ADCBL; i++){
		if(b2[i]<=triglvl && b2[i+1]>triglvl){
			trigp=i;
			break;
		}
	}
	//trigp=0;
}
