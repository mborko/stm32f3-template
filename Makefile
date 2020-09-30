# STM32F3-Discovery Makefile
#
# Part of the stm32f3-discovery project
#
#######################################

# TARGET: name of the output file
TARGET = main

# MCU: part number to build for
MCU = STM32F3XX
STM_SERIE=STM32F3XX
STM_MODEL=STM32F303xC
BSP_MODEL=STM32F3-Discovery

# OUTDIR: directory to use for output
OUTDIR = build
MAINFILE = $(OUTDIR)/$(TARGET).bin

# STM32_PATH: path to STM32 Firmware folder
STM32_PATH = $(HOME)/opt/STM32CubeF3
CMSIS_PATH = $(STM32_PATH)/Drivers/CMSIS

PROJ := src/template

# SOURCES: list of input source sources
SOURCEDIR = $(PROJ)/Src
SOURCES	+= $(shell find $(SOURCEDIR) -name '*.c')

SOURCES += default/stm32f3_discovery.c

ASM_SOURCES += $(CMSIS_PATH)/Device/ST/STM32F3xx/Source/Templates/gcc/startup_stm32f303xc.s

# INCLUDES: list of includes, by default, use Includes directory
INCLUDES = -Iinclude
INCLUDES += -I$(PROJ)/Inc
INCLUDES += -I$(STM32_PATH)/Drivers/STM32F3xx_HAL_Driver/Inc
INCLUDES += -I$(STM32_PATH)/Drivers/BSP/$(BSP_MODEL)
INCLUDES += -I$(STM32_PATH)/Projects/STM32F3-Discovery/Demonstrations/Inc
INCLUDES += -I$(STM32_PATH)/Projects/STM32F3-Discovery/Templates/Inc
INCLUDES += -I$(CMSIS_PATH)/Device/ST/STM32F3xx/Include
INCLUDES += -I$(CMSIS_PATH)/Include
##INCLUDES += -include$(STM32_PATH)/Project/Demonstration/stm32f30x_conf.h
INCLUDES += -include$(STM32_PATH)/Projects/STM32F3-Discovery/Templates/Inc/stm32f3xx_hal_conf.h
INCLUDES += -include$(STM32_PATH)/Drivers/BSP/STM32F3-Discovery/stm32f3_discovery.h
INCLUDES += -include$(CMSIS_PATH)/Device/ST/STM32F3xx/Include/stm32f3xx.h
INCLUDES += -I../

# LIBRARIES
HAL_LIBDIR	= $(STM32_PATH)/Drivers/STM32F3xx_HAL_Driver/Src
LIB_SOURCES	+= $(shell find $(HAL_LIBDIR) -maxdepth 1 -name '*.c')
LIB_ASM_SRC	+= $(shell find $(HAL_LIBDIR) -maxdepth 1 -name '*.S')
BSP_LIBDIR	= $(STM32_PATH)/Drivers/BSP/$(BSP_MODEL)
LIB_SOURCES	+= $(shell find $(BSP_LIBDIR) -maxdepth 1 -name '*.c')
LIB_OUTDIR	= lib/hal_build
STM32_LIB	= lib/libstm32_f3.a

# LD_SCRIPT: linker script
LD_SCRIPT = default/STM32F303VCTx_FLASH.ld

# define flags
##CFLAGS = -g -mthumb -mthumb-interwork -mcpu=cortex-m4
##CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
##CFLAGS += -Os -MD -std=c99 -Wall -Wextra #-pedantic
##CFLAGS += -fsingle-precision-constant -Wdouble-promotion
##CFLAGS += -ffunction-sections -fdata-sections
##CFLAGS += -D$(MCU) -DF_CPU=72000000 $(INCLUDES) -c
CFLAGS  = -ggdb -O0 -Wall -Wextra -Warray-bounds
CFLAGS += -mcpu=cortex-m4 -mthumb -mlittle-endian -mthumb-interwork
CFLAGS += -mfloat-abi=softfp -mfpu=fpv4-sp-d16
CFLAGS += -DUSE_STDPERIPH_DRIVER -D$(STM_SERIE) -D$(STM_MODEL) $(INCLUDES) -c

ASFLAGS = -x assembler-with-cpp -fmessage-length=0 -mcpu=cortex-m4 -mthumb -gdwarf-2

##LDFLAGS = -mcpu=cortex-m4 -mthumb -T $(LD_SCRIPT) -L. -nostdlib
##LDFLAGS += -Wl,--relax -Wl,--gc-sections
LDFLAGS  = -g -O2 -Wall -T$(LD_SCRIPT)
LDFLAGS += --specs=nosys.specs
LDFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork

#######################################
# output configs
#######################################
ifeq ($(NOCOLOR),1)
	CYAN            = ""
	NORMAL          = ""
else
	CYAN        = `tput setaf 6`
	NORMAL      = `tput sgr0`
endif
#######################################
# binaries
#######################################
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
OPENOCD = openocd
FLASH	= st-flash
RM      = rm -rf
MKDIR	= mkdir -p
#######################################

# list of object files, placed in the build directory regardless of source path
OBJECTS = $(addprefix $(OUTDIR)/,$(notdir $(SOURCES:.c=.o)))
ASM_OBJECTS = $(addprefix $(OUTDIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
LIB_OBJECTS = $(addprefix $(LIB_OUTDIR)/,$(notdir $(LIB_SOURCES:.c=.o)))

# default: build bin
all: $(STM32_LIB) $(OUTDIR)/$(TARGET).bin

$(OBJECTS): $(SOURCES) | $(OUTDIR)
	@echo -e "Compiling\t"$(CYAN)$(filter %$(subst .o,.c,$(@F)), $(SOURCES))$(NORMAL)
	@$(CC) $(CFLAGS) -o $@ $(filter %$(subst .o,.c,$(@F)), $(SOURCES))

$(ASM_OBJECTS): $(ASM_SOURCES) | $(OUTDIR)
	@echo -e "Assembling\t"$(CYAN)$(filter %$(subst .o,.s,$(@F)), $(ASM_SOURCES))$(NORMAL)
	@$(CC) $(ASFLAGS) -c $(filter %$(subst .o,.s,$(@F)), $(ASM_SOURCES)) -o $@

$(LIB_OBJECTS): $(LIB_SOURCES) | $(LIB_OUTDIR)
	@echo -e "Compiling\t"$(CYAN)$(filter %$(subst .o,.c,$(@F)), $(LIB_SOURCES))$(NORMAL)
	@$(CC) $(CFLAGS) -o $@ $(filter %$(subst .o,.c,$(@F)), $(LIB_SOURCES))

$(OUTDIR)/$(TARGET): $(OBJECTS) $(ASM_OBJECTS) $(STM32_LIB)
	@echo -e "Linking\t\t"$(CYAN)$^$(NORMAL)
	@$(LD) $(LDFLAGS) -o $@ $^

$(MAINFILE): $(OUTDIR)/$(TARGET)
	@$(OBJCOPY) -O binary $< $@

$(STM32_LIB): $(LIB_OBJECTS)
	@echo -e "Making library \t"$(CYAN)$@$(NORMAL)
	@$(AR) rs $(STM32_LIB) $(LIB_OBJECTS)

# create the output directory
$(OUTDIR):
	$(MKDIR) $(OUTDIR)

$(LIB_OUTDIR):
	$(MKDIR) $(LIB_OUTDIR)

program: $(MAINFILE) 
	$(OPENOCD) -f config/openocd.cfg

flash: $(MAINFILE)
	$(FLASH) --reset write $(MAINFILE) 0x8000000

debug: flash
	./debug/nemiver.sh $(TARGET)

cleanall:
	-$(RM) $(OUTDIR)
	-$(RM) $(LIB_OUTDIR)
	-$(RM) $(STM32_LIB)

clean:
	-$(RM) $(OUTDIR)/*

.PHONY: all clean
