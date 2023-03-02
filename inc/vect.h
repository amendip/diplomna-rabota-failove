#ifndef __VGA_VECT_H
#define __VGA_VECT_H

#include "stm32_ub_vga_screen.h"
#include "databuf.h"
#include "font.h"

void linehandler(void);
void linedraw(void);

extern uint16_t ia[321], la[481];
extern uint8_t rstrsi;
extern uint8_t adcbufbuf;
extern uint16_t trigp;
extern uint8_t banner[41];
extern uint8_t mode;

#define GBL 10//graph buffer length

#endif
