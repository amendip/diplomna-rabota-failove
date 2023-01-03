#include "databuf.h"

uint16_t wb1[320]={
0x800,0x828,0x850,0x879,0x8a1,0x8c9,0x8f1,0x919,0x940,0x968,0x990,0x9b7,0x9de,0xa05,0xa2c,0xa53,0xa79,0xa9f,0xac5,0xaea,0xb10,0xb35,0xb59,0xb7e,0xba2,0xbc5,0xbe9,0xc0c,0xc2e,0xc50,0xc72,0xc93,
0xcb4,0xcd4,0xcf4,0xd13,0xd32,0xd50,0xd6e,0xd8b,0xda8,0xdc4,0xde0,0xdfb,0xe15,0xe2f,0xe48,0xe61,0xe79,0xe90,0xea7,0xebd,0xed2,0xee7,0xefb,0xf0e,0xf21,0xf33,0xf44,0xf54,0xf64,0xf73,0xf81,0xf8f,
0xf9c,0xfa8,0xfb3,0xfbe,0xfc7,0xfd0,0xfd9,0xfe0,0xfe7,0xfed,0xff2,0xff6,0xffa,0xffc,0xffe,0x1000,0x1000,0x1000,0xffe,0xffc,0xffa,0xff6,0xff2,0xfed,0xfe7,0xfe0,0xfd9,0xfd0,0xfc7,0xfbe,0xfb3,0xfa8,
0xf9c,0xf8f,0xf81,0xf73,0xf64,0xf54,0xf44,0xf33,0xf21,0xf0e,0xefb,0xee7,0xed2,0xebd,0xea7,0xe90,0xe79,0xe61,0xe48,0xe2f,0xe15,0xdfb,0xde0,0xdc4,0xda8,0xd8b,0xd6e,0xd50,0xd32,0xd13,0xcf4,0xcd4,
0xcb4,0xc93,0xc72,0xc50,0xc2e,0xc0c,0xbe9,0xbc5,0xba2,0xb7e,0xb59,0xb35,0xb10,0xaea,0xac5,0xa9f,0xa79,0xa53,0xa2c,0xa05,0x9de,0x9b7,0x990,0x968,0x940,0x919,0x8f1,0x8c9,0x8a1,0x879,0x850,0x828,
0x800,0x7d8,0x7b0,0x787,0x75f,0x737,0x70f,0x6e7,0x6c0,0x698,0x670,0x649,0x622,0x5fb,0x5d4,0x5ad,0x587,0x561,0x53b,0x516,0x4f0,0x4cb,0x4a7,0x482,0x45e,0x43b,0x417,0x3f4,0x3d2,0x3b0,0x38e,0x36d,
0x34c,0x32c,0x30c,0x2ed,0x2ce,0x2b0,0x292,0x275,0x258,0x23c,0x220,0x205,0x1eb,0x1d1,0x1b8,0x19f,0x187,0x170,0x159,0x143,0x12e,0x119,0x105,0xf2,0xdf,0xcd,0xbc,0xac,0x9c,0x8d,0x7f,0x71,
0x64,0x58,0x4d,0x42,0x39,0x30,0x27,0x20,0x19,0x13,0xe,0xa,0x6,0x4,0x2,0x0,0x0,0x0,0x2,0x4,0x6,0xa,0xe,0x13,0x19,0x20,0x27,0x30,0x39,0x42,0x4d,0x58,
0x64,0x71,0x7f,0x8d,0x9c,0xac,0xbc,0xcd,0xdf,0xf2,0x105,0x119,0x12e,0x143,0x159,0x170,0x187,0x19f,0x1b8,0x1d1,0x1eb,0x205,0x220,0x23c,0x258,0x275,0x292,0x2b0,0x2ce,0x2ed,0x30c,0x32c,
0x34c,0x36d,0x38e,0x3b0,0x3d2,0x3f4,0x417,0x43b,0x45e,0x482,0x4a7,0x4cb,0x4f0,0x516,0x53b,0x561,0x587,0x5ad,0x5d4,0x5fb,0x622,0x649,0x670,0x698,0x6c0,0x6e7,0x70f,0x737,0x75f,0x787,0x7b0,0x7d8
};

uint8_t lb1[ADCBL][2]={0};
uint16_t wb2[2*ADCBL]={0};
uint16_t wb3[2*ADCBL]={0};
uint16_t wb4[2*ADCBL]={0};
uint16_t wb2b[2*ADCBL]={0};
uint16_t wb3b[2*ADCBL]={0};
uint16_t wb4b[2*ADCBL]={0};

uint8_t textbuf[16][200]={0};

void wb2iala(){
uint8_t lf=0;
uint16_t iai=0, flf=0;
ia[320]=0xffff;
la[480]=320;
for(uint16_t y=0;y<480;y++){
	lf=0;
	for(uint16_t i=0;i<320;i++){
		if(y==(wb2[i]>>4)){
			ia[iai]=i;
			if(!lf){
			//la[y]=iai;
			lf=1;
			//flf=1;
			}
			iai++;
		}
		//ia[i]=i;
	}
	//if(y<320) flf=y; else flf=320;
	//if(y<476) 
	//la[y]=flf;

	//if(!lf){
	//if(!flf) la[y]=0;
	//else la[y]=0xffff;
	//la[y]=320;
	//}
}
//la[0]=0;
//la[1]=0;
//la[2]=0;
//la[3]=100;
//la[4]=200;

for(int16_t lai=480;lai>0;lai--){
//	if(la[lai-1]==0xffff) la[lai-1]=la[lai];
	la[lai]=320;
}
la[0]=0;

}
