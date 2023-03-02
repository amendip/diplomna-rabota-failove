#include "vect.h"

uint8_t g1e=1, g2e=0, g3e=0;
uint16_t g1b[GBL]={0x1,0x2,0x3,0x2,0x1,0,0,5,0,0}, g2b[GBL], g3b[GBL];

//uint8_t banner[41]="Tova e tekst abcdef 123456?";
//uint8_t banner[41]="U1=20V U2=30V U3=1.5V";
uint8_t banner[41]="U1=20V U2=000 U3=000";


uint8_t rv0_1=0;
uint8_t rstrsi=0;
uint8_t adcbufbuf=0;
uint16_t trigp=0;
uint8_t VGA_RAMvect0[321];
uint8_t VGA_RAMvect1[321];

uint8_t mode=1;
uint16_t xyc=0;

void linehandler(){
	if(!rv0_1)
	VGA.start_adr=&VGA_RAMvect0[0];
	else
	VGA.start_adr=&VGA_RAMvect1[0];
}
#define SCRW 240
#define GRID 0x01010101
void linedraw(){
	uint8_t *vadr;
	uint8_t scm, tcm;
	uint32_t linecol;
	uint16_t line=VGA.hsync_cnt-VGA_VSYNC_BILD_START;
	uint16_t *b2, *b3, *b4;
	if(adcbufbuf) b2=wb2, b3=wb3, b4=wb4; else b2=wb2b, b3=wb3b, b4=wb4b;
	if(!rv0_1)
	vadr=&VGA_RAMvect1[0];
	else
	vadr=&VGA_RAMvect0[0];

	vadr[SCRW]=0;
	if(line%48) linecol=0x00000000; else linecol=GRID;
	if(line!=17) for(uint16_t i=0; i<SCRW/4; i++)
		((uint32_t *)vadr)[i]=linecol;
	//for(uint16_t i=la[line]; i<la[line+1]; i++){
	//	if(200-10*g1b[i%GBL]==VGA.hsync_cnt-VGA_VSYNC_BILD_START)
	//	if(200==VGA.hsync_cnt-VGA_VSYNC_BILD_START)
	//	vadr[i]=0xFC;
	if(line==0){
	for(uint16_t i=0; i<SCRW; i++)
		vadr[i]=0xff;
	}else if(line<16){
		for(uint16_t i=0; i<50; i++)
		((uint32_t *)vadr)[i]=((uint32_t *)textbuf[line])[i];
		//vadr[32]=0xff;
		vadr[230]=0xff;
	}else if(line==17){
	for(uint16_t i=0; i<SCRW; i++)
		vadr[i]=0xff;
	}else{
	for(uint16_t i=35;i<280;i+=36) vadr[i]=GRID;
	scm=0xE3*(adccc>1);
	tcm=0x1C*(adccc>2);
	b2=&b2[trigp];
	b3=&b3[trigp];
	b4=&b4[trigp];
	
	switch(mode){
	case 0:
	for(uint16_t i=rstrsi;i<SCRW;i+=4){
		//if(line==300-(wb1[(i<<2)&0xff]>>4))
		//vadr[i]^=0xFF;
		//if(line==300-(wb2[i]>>4))
		//if(line==300-((wb2[i]>>4) + (i>>5)))
		//if(line==((uint16_t)300-(wb1[i]>>4)))
		//if(line==(300-((b2[trigp+i])>>4)))
		if(line==b2[i])
		//if(line>=(300-(lb1[i][0])) && line<=(300-(lb1[i][1])))
		//vadr[i]^=0xE3;
		//vadr[i]^=0x1C;
		vadr[i]^=0xFC;
		if(line==b3[i])//{
		//vadr[i]^=0xE0;
		vadr[i]^=scm;
		if(line==b4[i])
		vadr[i]^=tcm;
		// }
		//if(line==(300+((wb3[i])>>4)))
		//vadr[i]^=0xE3;
		//if(line==(300-((wb1[i])>>4)))
		//vadr[i]^=0xE0;
		///vadr[i]^=0xE0;
		//if(line==300-(wb1[i]>>5))
		//vadr[i]^=0xFF;
	//	vadr[ia[i]]^=0xfc;
	}
	break;
	case 1:
	for(;xyc<ADCBL;xyc+=1){
		if(b2[xyc]!=line) break;
		vadr[b3[xyc]]=b4[xyc];
	}
	if(xyc>=ADCBL) xyc=0;
	
	break;
	}

	}
	//for(uint16_t i=la2[line]; i<la2[line+1]; i++)
	//	vadr[ia2[i]]^=0xE3;
	//for(uint16_t i=la[line]; i<la[line+1]; i++)
	//	vadr[ia2[i]]^=0x1F;
	rv0_1=!rv0_1;
}
