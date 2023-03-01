#ifndef __ADCC_H
#define __ADCC_H

#include "stm32f4xx_adc.h"
#include "databuf.h"

void adcpininit(void);
void adcpolling(void);
uint16_t adcsample(void);
void hsyncsample(void);

extern uint16_t adccnt;
extern uint8_t adcend;
extern uint8_t adccc;
extern uint8_t adcpause;
extern uint8_t adcbuf, adcbad, adcbbd;

#endif
