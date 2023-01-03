# path to STM32F103 standard peripheral library
STD_PERIPH_LIBS ?= ./STM32F4xx_DSP_StdPeriph_Lib_V1.9.0/Libraries/STM32F4xx_StdPeriph_Driver
STD_PERIPH_LIBS_CMSIS ?= ./STM32F4xx_DSP_StdPeriph_Lib_V1.9.0/Libraries/CMSIS
SRC ?= ./src

# list of source files
SOURCES  = $(SRC)/main.c
SOURCES += $(SRC)/vect.c
SOURCES += $(SRC)/font.c
SOURCES += $(SRC)/adc.c
SOURCES += $(SRC)/databuf.c
SOURCES += $(SRC)/stm32_ub_vga_screen.c
SOURCES += $(STD_PERIPH_LIBS)/src/stm32f4xx_rcc.c
SOURCES += $(STD_PERIPH_LIBS)/src/stm32f4xx_gpio.c
SOURCES += $(STD_PERIPH_LIBS)/src/stm32f4xx_rcc.c
SOURCES += $(STD_PERIPH_LIBS)/src/stm32f4xx_tim.c
SOURCES += $(STD_PERIPH_LIBS)/src/misc.c
SOURCES += $(STD_PERIPH_LIBS)/src/stm32f4xx_dma.c
SOURCES += $(STD_PERIPH_LIBS)/src/stm32f4xx_adc.c
SOURCES += $(STD_PERIPH_LIBS_CMSIS)/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
SOURCES += $(SRC)/startup_stm32f401xx.s
#SOURCES += $(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/TrueSTUDIO/startup_stm32f10x_md.s

# name for output binary files
PROJECT ?= vga

# compiler, objcopy (should be in PATH)
CC = arm-unknown-eabi-gcc
OBJCOPY = arm-unknown-eabi-objcopy

# path to st-flash (or should be specified in PATH)
ST_FLASH ?= st-flash

# specify compiler flags
CFLAGS  = -g -O2 -Wall
#CFLAGS += -T$(STD_PERIPH_LIBS)/Project/STM32F10x_StdPeriph_Template/TrueSTUDIO/STM3210B-EVAL/stm32_flash.ld
#CFLAGS += -T$(STD_PERIPH_LIBS)/Project/STM32F4xx_StdPeriph_Templates/TrueSTUDIO/STM32F401xx/STM32F401VC_FLASH.ld
CFLAGS += -TSTM32F401CCUX_FLASH.ld
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
#CFLAGS += -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
CFLAGS += -DSTM32F401xx -DSTM32F401xC -DUSE_STDPERIPH_DRIVER
CFLAGS += -Wl,--gc-sections
CFLAGS += -I.
#CFLAGS += -I$(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/
CFLAGS += -I$(STD_PERIPH_LIBS_CMSIS)/Device/ST/STM32F4xx/Include/
CFLAGS += -I$(STD_PERIPH_LIBS_CMSIS)/Include/
#CFLAGS += -I$(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/CoreSupport
CFLAGS += -I$(STD_PERIPH_LIBS)/inc
CFLAGS += -Iinc
CFLAGS += -g

OBJS = $(SOURCES:.c=.o)

all: $(PROJECT).elf

# compile
$(PROJECT).elf: $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@
	#$(OBJCOPY) -O ihex $(PROJECT).elf $(PROJECT).hex
	#$(OBJCOPY) -O binary $(PROJECT).elf $(PROJECT).bin

# remove binary files
clean:
	rm -f *.o *.elf *.hex *.bin

# flash
burn:
	sudo $(ST_FLASH) write $(PROJECT).bin 0x8000000
