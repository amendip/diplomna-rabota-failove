//--------------------------------------------------------------
// File     : stm32_ub_vga_screen.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_VGA_SCREEN_H
#define __STM32F4_UB_VGA_SCREEN_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "stm32f4xx_dma.h"
#include "vect.h"
#include "adc.h"

//--------------------------------------------------------------
// Defines für die Auflösungen :
// der VGA-Monitor wird im Mode 640x480 betrieben
// das angezeigte Bild besteht aber nur aus 320x240 Pixel
// jedes Pixel wird als 2x2 Punkt am Monitor angezeigt
//--------------------------------------------------------------
#define VGA_DISPLAY_X   320  // Anzahl der Pixel in X-Richtung
#define VGA_DISPLAY_Y   60  // Anzahl der Pixel in Y-Richtung



//--------------------------------------------------------------
// VGA Struktur
//--------------------------------------------------------------
typedef struct {
  uint16_t hsync_cnt;   // Zeilenzähler
  uint32_t start_adr;   // start_adresse
  uint32_t dma2_cr_reg; // Inhalt vom CR-Register
}VGA_t;
//VGA_t VGA;

extern VGA_t VGA;
extern uint8_t nhsps, invblank;


//--------------------------------------------------------------
// Timer-1
// Funktion  = Pixelclock (Takt für den DMA Transver)
//
// Grundfreq = 2*APB2 (APB2=84MHz) => TIM_CLK=168MHz
// Frq       = 168MHz/1/12 = 14MHz
//
// mit diesem Wert kann die "breite" des angezeigten Bildes 
// beeinflusst werden
//--------------------------------------------------------------
//#define VGA_TIM1_PERIODE      11
#define VGA_TIM1_PERIODE       5
#define VGA_TIM1_PRESCALE      0



//--------------------------------------------------------------
// Timer-2
// Funktion  = CH4 : HSync-Signal erzeugen an PB11
//             CH3 : Triggerpunkt für DMA start
//
// Grundfreq = 2*APB1 (APB1=48MHz) => TIM_CLK=84MHz
// Frq       = 84MHz/1/2668 = 31,48kHz => T = 31,76us
// 1TIC      = 11,90ns
//
// der Wert vom DMA-Delay muss ausprobiert werden
// und ist auch abhängig von der Optimierungsstufe
//   grössere Zahlen verschieben das Bild nach links
//   kleinere Zahlen verschieben das Bild nach rechts
//--------------------------------------------------------------
#define  VGA_TIM2_HSYNC_PERIODE   2667
#define  VGA_TIM2_HSYNC_PRESCALE     0

#define  VGA_TIM2_HSYNC_IMP       320  // HSync-Impulslänge (3,81us)
#define  VGA_TIM2_HTRIGGER_START  480  // HSync+BackPorch (5,71us)
#define  VGA_TIM2_FRONT_PORCH  2615  // HSync+BackPorch (5,71us)
#define  VGA_TIM5_VIDEO  2135  // HSync+BackPorch (5,71us)
//#define  VGA_TIM2_DMA_DELAY        60  // gleicht das Delay vom DMA START aus (Optimization = none)
#define  VGA_TIM2_DMA_DELAY        93
//#define  VGA_TIM2_DMA_DELAY        144


//--------------------------------------------------------------
// VSync-Signal
// Trigger   = Timer2 Update (f=31,48kHz => T = 31,76us)
// 1TIC      = 31,76us
//--------------------------------------------------------------
#define  VGA_VSYNC_PERIODE        525   // Anzahl der Zeilen pro Bild

#define  VGA_VSYNC_IMP  2               // VSync-Impulslänge (63,64us)
#define  VGA_VSYNC_BILD_START      36   // VSync+BackPorch (1,11ms)
#define  VGA_VSYNC_BILD_STOP      514   // VSync+BackPorch+Bildlänge (16,38ms)



#define VGA_GPIOB_ODR_ADDRESS  ((uint32_t)0x40020414) // ADR von Port-B





#define VGA_GPIO_HINIBBLE  ((uint16_t)0x00FF) // GPIO_Pin_8 bis GPIO_Pin_15





void UB_VGA_Screen_Init(void);


#endif // __STM32F4_UB_VGA_SCREEN_H
