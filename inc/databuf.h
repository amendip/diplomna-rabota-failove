#ifndef __DATABUF_H
#define __DATABUF_H

#include "vect.h"

#define ADCBL 320

void wb2iala(void);

extern uint8_t lb1[2*ADCBL][2], textbuf[16][200];
//extern uint16_t wb1[ADCBL], wb2[2*ADCBL], wb3[2*ADCBL], wb4[2*ADCBL];
//extern uint16_t wb2b[2*ADCBL], wb3b[2*ADCBL], wb4b[2*ADCBL];
extern uint16_t wbc[3][2*ADCBL];
extern uint16_t *wb2, *wb3, *wb4;
extern uint16_t wbcb[3][2*ADCBL];
extern uint16_t *wb2b, *wb3b, *wb4b;

#endif
