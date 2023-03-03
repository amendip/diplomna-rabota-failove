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
    haha=100000;
  }

  if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)){
   if(!bp){
   //banner[3]++;
   bp=60000;
   if(nhsps<200) nhsps++; else nhsps=1;
   mode=(mode+1)%3;
   }
  }else if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)){
   if(!bp){
   //banner[3]++;
   bp=60000;
   if(nhsps>0) nhsps--; else nhsps=200;
   mode=(mode+2)%3;
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

void swp(uint16_t *a, uint16_t *b){
	uint16_t tmp;
	tmp=*a;
	*a=*b;
	*b=tmp;
}

float bclv(float a){
float b=4-a;
if(a<b && a<1) return 1-a;
if(b<a && b<1) if(b>0) return 1-b; else return 1;
return 0;
}

float fm(float a, uint8_t b){
return a-((uint8_t)(a/b))*b;
}

uint8_t hue2rgb(uint16_t hue){
float h;
float kr, kg, kb;
	h=(hue>>4)*0.02380952381;
	kr=fm((5+h),6);
	kg=fm((3+h),6);
	kb=fm((1+h),6);
	return ((uint8_t)(bclv(kr)*0x7)<<5)|((uint8_t)(bclv(kg)*0x7)<<2)|(uint8_t)(bclv(kb)*0x3);
}

void auh(uint16_t *a, uint8_t b){
*a&=0xFF;
*a|=b<<8;
}

void fft(uint16_t *a, uint16_t l, uint16_t s){
int16_t r, im;
uint8_t a1, a2;

if(s<l){
fft(a, l, s*2);
fft(a+s, l, s*2);

for(uint16_t i=0;i<l;i+=2*s){

if(i>=2*ADCBL){
continue;
}else{
r=cos(-M_PI*i/l)*a[i+s];
im=sin(-M_PI*i/l)*a[i+s];
a1=sqrt(((a[i]&0xFF)+r)*((a[i]&0xFF)+r)+im*im);
a2=sqrt(((a[i]&0xFF)-r)*((a[i]&0xFF)-r)+im*im);
}

auh(&a[i/2], a1);
auh(&a[(i+l)/2], a2);
}
}

}

/*
void four1(uint16_t *data, unsigned long l)
{
unsigned long n, mmax, m, j, istep, i;
double wtemp, wr, wpr, wpi, wi, theta;
double tempr, tempi;
// reverse-binary reindexing
n = l<<1;
j=1;
for (i=1; i<n; i+=2) {
if (j>i) {
if(i>=ADCBL || j>=ADCBL) continue;
swp(&data[j-1], &data[i-1]);
swp(&data[j], &data[i]);
}
m = l;
while (m>=2 && j>m) {
j -= m;
m >>= 1;
}
j += m;
};

// here begins the Danielson-Lanczos section
mmax=2;
while (n>mmax) {
istep = mmax<<1;
theta = -(2*M_PI/mmax);
wtemp = sin(0.5*theta);
wpr = -2.0*wtemp*wtemp;
wpi = sin(theta);
wr = 1.0;
wi = 0.0;
for (m=1; m < mmax; m += 2) {
for (i=m; i <= n; i += istep) {
j=i+mmax;
if(i>=ADCBL || j>=ADCBL) continue;
tempr = wr*data[j-1] - wi*data[j];
tempi = wr * data[j] + wi*data[j-1];

data[j-1] = data[i-1] - tempr;
data[j] = data[i] - tempi;
data[i-1] += tempr;
data[i] += tempi;
}
wtemp=wr;
wr += wr*wpr - wi*wpi;
wi += wi*wpr + wtemp*wpi;
}
mmax=istep;
}
}
*/

void four1(int16_t *data, const int n) {
int nn,mmax,m,j,istep,i;
double wtemp,wr,wpr,wpi,wi,theta,tempr,tempi;

nn = n << 1;
j = 1;
for (i=1;i<nn;i+=2) {
if (j > i) {
if(i>=2*ADCBL || j>=2*ADCBL) continue;
swp(&data[j-1],&data[i-1]);
swp(&data[j],&data[i]);
}
m=n;
while (m >= 2 && j > m) {
j -= m;

m >>= 1;
}
j += m;
}

mmax=2;
while (nn > mmax) {
istep=mmax << 1;
theta=(6.28318530717959/mmax);
wtemp=sin(0.5*theta);
wpr = -2.0*wtemp*wtemp;
wpi=sin(theta);
wr=1.0;
wi=0.0;
for (m=1;m<mmax;m+=2) {
for (i=m;i<=nn;i+=istep) {
j=i+mmax;
if(i>=2*ADCBL || j>=2*ADCBL) continue;
tempr=wr*data[j-1]-wi*data[j];
tempi=wr*data[j]+wi*data[j-1];
data[j-1]=data[i-1]-tempr;
data[j]=data[i]-tempi;
data[i-1] += tempr;
data[i] += tempi;
}
wr=(wtemp=wr)*wpr-wi*wpi+wr;
wi=wi*wpr+wtemp*wpi+wi;
}
mmax=istep;
}
}

void realft(int16_t *data, unsigned long n) {
int i,i1,i2,i3,i4;
double c1=0.5,c2,h1r,h1i,h2r,h2i,wr,wi,wpr,wpi,wtemp;
double theta=3.141592653589793238/(n>>1);

c2 = -0.5;
four1(data,n>>1);

wtemp=sin(0.5*theta);
wpr = -2.0*wtemp*wtemp;
wpi=sin(theta);
wr=1.0+wpr;
wi=wpi;

for (i=1;i<(n>>2);i++) {
i2=1+(i1=i+i);
i4=1+(i3=n-i1);
if(i1>=2*ADCBL || i2>=2*ADCBL || i3>=2*ADCBL || i4>=2*ADCBL) continue;
h1r=c1*(data[i1]+data[i3]);
h1i=c1*(data[i2]-data[i4]);
h2r= -c2*(data[i2]+data[i4]);
h2i=c2*(data[i1]-data[i3]);
data[i1]=h1r+wr*h2r-wi*h2i;
data[i2]=h1i+wr*h2i+wi*h2r;
data[i3]=h1r-wr*h2r+wi*h2i;
data[i4]= -h1i+wr*h2i+wi*h2r;
wr=(wtemp=wr)*wpr-wi*wpi+wr;
wi=wi*wpr+wtemp*wpi+wi;
}

data[0] = (h1r=data[0])+data[1];
data[1] = h1r-data[1];

}

#define pw2c(x) (int16_t)x*(int16_t)x

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
	nhsps=(((ADC1->DR)>>5)+1);


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
	
	switch(mode){
	case 0:
	triglvl=((uint32_t)min+(uint32_t)max)/(uint32_t)4;
	//triglvl=0x1FF;
	for(uint16_t i=0; i<ADCBL; i++){
		if(b2[i]<=triglvl && b2[i+1]>triglvl){
			trigp=i;
			break;
		}
	}
	if(trigp==0) banner[0]++;

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
	for(uint16_t i=0; i<2*ADCBL; i++){
	maxi=i;
	for(uint16_t j=i+1; j<2*ADCBL; j++)
	if(b2[j]>b2[maxi])
	maxi=j;


	swp(&b2[i],&b2[maxi]);
	swp(&b3[i],&b3[maxi]);
	swp(&b4[i],&b4[maxi]);

	b2[i]=300-(b2[i]>>4);
	b3[i]=150-(b3[i]>>5);
	//b3[i]=150;
	//b4[i]=((b4[i]>>4)|0x3);
	b4[i]=hue2rgb(b4[i]);
	//b4[i]=0xFF;
	}
	break;
	
	case 2:
	trigp=0;
	for(uint16_t i=0; i<2*ADCBL; i++){
	if(b2[i]&1){ b2[i]=0; continue;}
	b2[i]>>=6;
	}
	//fft(b2, 1024, 1);
	//four1(b2, 256);
	realft(b2, 512);
	for(uint16_t i=0; i<ADCBL; i+=2){
	//if(i>512){
	//b2[i>>1]=301;
	//continue;
	//}
	b2[i]=sqrt(pw2c(b2[i])+pw2c(b2[i+1]));
	//b2[i>>1]>>=8;
	b2[i]=300-((b2[i])>>6);
	b2[i+1]=600;

	/*
	if(((int16_t)b2[i])>0)
	b2[i]=300-((b2[i])>>6);
	//b2[i>>1]=300-((b2[i>>1])<<2);
	else
	b2[i]=300-(((int16_t)b2[i])/64);
	if(((int16_t)b2[i+1])>0)
	b2[i+1]=300-((b2[i+1])>>6);
	else
	b2[i+1]=300-(((int16_t)b2[i+1])/64);
	*/

	//b2[i]=350;
	//b2[i]=b2[i+1]=0;
	//b2[i+1]=340;
	}
	break;

	}
	adcbufbuf=adcbuf;
	adcend=0;
	}
}
