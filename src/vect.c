#include "vect.h"

uint8_t g1e=1, g2e=0, g3e=0;
uint16_t g1b[GBL]={0x1,0x2,0x3,0x2,0x1,0,0,5,0,0}, g2b[GBL], g3b[GBL];

//uint8_t banner[41]="Tova e tekst abcdef 123456?";
uint8_t banner[41]="U1=20V U2=30V U3=1.5V";


uint8_t rv0_1=0;
uint8_t rstrsi=0;
uint8_t adcbufbuf=0;
uint16_t trigp=0;
uint8_t VGA_RAMvect0[321];
uint8_t VGA_RAMvect1[321];

void linehandler(){
	if(!rv0_1)
	VGA.start_adr=&VGA_RAMvect0[0];
	else
	VGA.start_adr=&VGA_RAMvect1[0];
}
#define SCRW 300
void linedraw(){
	uint8_t *vadr;
	uint16_t line=VGA.hsync_cnt-VGA_VSYNC_BILD_START;
	uint16_t *b2;
	if(adcbufbuf) b2=wb2; else b2=wb2b;
	if(!rv0_1)
	vadr=&VGA_RAMvect1[0];
	else
	vadr=&VGA_RAMvect0[0];
	if(line!=17) for(uint16_t i=0; i<SCRW/4; i++)
		((uint32_t *)vadr)[i]=0x00;
	//for(uint16_t i=la[line]; i<la[line+1]; i++){
	//	if(200-10*g1b[i%GBL]==VGA.hsync_cnt-VGA_VSYNC_BILD_START)
	//	if(200==VGA.hsync_cnt-VGA_VSYNC_BILD_START)
	//	vadr[i]=0xFC;
	if(line==0){
	for(uint16_t i=0; i<SCRW; i++)
		vadr[i]=0xff;
	}else if(line<17){
		for(uint16_t i=0; i<50; i++)
		((uint32_t *)vadr)[i]=((uint32_t *)textbuf[line])[i];
		//vadr[32]=0xff;
		vadr[280]=0xff;
	}else if(line==17){
	for(uint16_t i=0; i<SCRW; i++)
		vadr[i]=0xff;
	}else for(uint16_t i=rstrsi;i<SCRW;i+=4){
		//if(line==300-(wb1[(i<<2)&0xff]>>4))
		//vadr[i]^=0xFF;
		//if(line==300-(wb2[i]>>4))
		//if(line==300-((wb2[i]>>4) + (i>>5)))
		//if(line==((uint16_t)300-(wb1[i]>>4)))
		if(line==(300-((b2[trigp+i])>>4)))
		//if(line>=(300-(lb1[i][0])) && line<=(300-(lb1[i][1])))
		//vadr[i]^=0xE3;
		vadr[i]^=0x1C;
		//if(line==(300+((wb3[i])>>4)))
		//vadr[i]^=0xE3;
		//if(line==(300-((wb1[i])>>4)))
		//vadr[i]^=0xE0;
		///vadr[i]^=0xE0;
		//if(line==300-(wb1[i]>>5))
		//vadr[i]^=0xFF;
	//	vadr[ia[i]]^=0xfc;
	}
	//for(uint16_t i=la2[line]; i<la2[line+1]; i++)
	//	vadr[ia2[i]]^=0xE3;
	//for(uint16_t i=la[line]; i<la[line+1]; i++)
	//	vadr[ia2[i]]^=0x1F;
	rv0_1=!rv0_1;
}
