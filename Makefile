
######################################
# target
######################################
TARGET = super-seq

FLASH_SIZE = $$((256 * 1024)) # 256 kB (Sector 6 and 7 used for config data)
RAM_SIZE = $$((128 * 1024)) # 128 kB

######################################
# building variables
######################################
# debug build?
DEBUG = 1

SERIAL_DEBUG ?= 0

# optimization
OPT = -Og

# get firmware version from git
FIRMWARE_VERSION = nope

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################

# SRCDIRS = SuperSeq STM32F4

# # Source files to compile
# CFILES = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
# CPPFILES = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.cpp))
# OBJFILES = $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)


######################################
# source
######################################
# C sources

C_SOURCES += \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
STM32F4/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_exti.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
STM32F4/middleware/FreeRTOS/Source/croutine.c \
STM32F4/middleware/FreeRTOS/Source/event_groups.c \
STM32F4/middleware/FreeRTOS/Source/list.c \
STM32F4/middleware/FreeRTOS/Source/queue.c \
STM32F4/middleware/FreeRTOS/Source/stream_buffer.c \
STM32F4/middleware/FreeRTOS/Source/tasks.c \
STM32F4/middleware/FreeRTOS/Source/timers.c \
STM32F4/middleware/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c \
STM32F4/middleware/FreeRTOS/Source/portable/MemMang/heap_4.c \
STM32F4/middleware/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c \
STM32F4/system/Src/freertos.c \
STM32F4/system/Src/stm32f4xx_hal_timebase_tim.c \
STM32F4/system/Src/stm32f4xx_hal_msp.c \
STM32F4/system/Src/stm32f4xx_it.c \
STM32F4/system/Src/system_stm32f4xx.c \
STM32F4/system/Src/system_clock_config.c

CPP_SOURCES += \
STM32F4/HAL/Src/gpio_api.cpp \
STM32F4/HAL/Src/error_handler.cpp \
STM32F4/HAL/Src/Flash.cpp \
STM32F4/HAL/Src/I2C.cpp \
STM32F4/HAL/Src/InterruptIn.cpp \
STM32F4/HAL/Src/logger.cpp \
STM32F4/HAL/Src/SPI.cpp \
STM32F4/HAL/Src/DigitalIn.cpp \
STM32F4/HAL/Src/DigitalOut.cpp \
STM32F4/HAL/Src/SuperClock.cpp \
STM32F4/HAL/Src/tim_api.cpp \
STM32F4/HAL/rtos/Src/SoftwareTimer.cpp \
STM32F4/HAL/rtos/Src/Mutex.cpp \
ok-drivers/drivers/CAP1208/CAP1208.cpp \
ok-drivers/drivers/DAC8554/DAC8554.cpp \
ok-drivers/drivers/SX1509/SX1509.cpp \
ok-drivers/drivers/IS31FL3739/IS31FL3739.cpp \
ok-drivers/drivers/MPR121/MPR121.cpp \
ok-drivers/drivers/MCP23017/MCP23017.cpp \
ok-drivers/utils/Algorithms/Algorithms.cpp \
ok-drivers/utils/ArrayMethods/ArrayMethods.cpp \
ok-drivers/utils/BitwiseMethods/BitwiseMethods.cpp \
SuperSeq/Src/main.cpp

# C includes
C_INCLUDES += \
-ISTM32F4/HAL/Inc \
-ISTM32F4/HAL/rtos/Inc \
-ISTM32F4/cxxsupport \
-ISTM32F4/STM32F4xx_HAL_Driver/Inc \
-ISTM32F4/STM32F4xx_HAL_Driver/Inc/Legacy \
-ISTM32F4/CMSIS/Device/ST/STM32F4xx/Include \
-ISTM32F4/CMSIS/Include \
-ISTM32F4/middleware/FreeRTOS/Source/include \
-ISTM32F4/middleware/FreeRTOS/Source/CMSIS_RTOS_V2 \
-ISTM32F4/middleware/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-ISTM32F4/system/Inc \
-Iok-drivers/drivers/CAP1208 \
-Iok-drivers/drivers/DAC8554 \
-Iok-drivers/drivers/SX1509 \
-Iok-drivers/drivers/IS31FL3739 \
-Iok-drivers/drivers/MPR121 \
-Iok-drivers/drivers/MCP23017 \
-Iok-drivers/drivers/TCA9548A \
-Iok-drivers/utils/Algorithms \
-Iok-drivers/utils/ArrayMethods \
-Iok-drivers/utils/BitwiseMethods \
-Iok-drivers/utils/OK_I2C \
-ISuperSeq/Inc \

# C++ includes
CPP_INCLUDES = \

# AS includes
AS_INCLUDES = 

# ASM sources ("Assembly Language") - defines main() function
ASM_SOURCES =  \
STM32F4/startup_stm32f446xx.s

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F4/STM32F446RETx_FLASH.ld


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
AR = $(GCC_PATH)/$(PREFIX)ar
GDB = $(GCC_PATH)/$(PREFIX)gdb
else
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
AR = $(PREFIX)ar
GDB = $(PREFIX)gdb
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################

# Specify the name of the target CPU.
CPU = -mcpu=cortex-m4

# Specify the name of the target floating point hardware/format.
FPU = -mfpu=fpv4-sp-d16

# Specify if floating point hardware should be used.
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F446xx



###########

# -Og                   
# -Wall	Recommended compiler warnings
# -fdata-sections
# -ffunction-sections
# -g    Generate debugging information
# -gdwarf-2
# -MMD
# -MP
# -c                       Compile and assemble, but do not link.
###########

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections 

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

ifeq ($(SERIAL_DEBUG), 1)
CFLAGS += -DSERIAL_DEBUG=1
endif

# pass the firmware version into program
CFLAGS += -DFIRMWARE_VERSION=\"$(FIRMWARE_VERSION)\"

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# C++ Flags
CPPFLAGS = $(CFLAGS) $(CPP_INCLUDES)
CPPFLAGS += \
-fno-exceptions \
-fno-rtti 

C_STANDARD = -std=gnu11
CPP_STANDARD += -std=gnu++14

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# helpers
#######################################
usedFlash = $$( $(SZ) $@ | sed -n 2p | awk '{print $$1}' )
usedFlashPercent = $$(( 100 * $(usedFlash) / $(FLASH_SIZE) ))
flashMessage = Flash Used: $(usedFlash)/$(FLASH_SIZE) ( $(usedFlashPercent) % )
usedRam = $$( $(SZ) $@ | sed -n 2p | awk '{ram=$$2+$$3} {print ram}' )
usedRamPercent = $$(( 100 * $(usedRam) / $(RAM_SIZE) ))
ramMessage = Ram Used: $(usedRam)/$(RAM_SIZE) ( $(usedRamPercent) % ) - (static only)

#######################################
# build the application
#######################################
# list of .c objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of .cpp objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(CPP_SOURCES:.cpp=.o))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@
	@echo ""
	@echo "$(flashMessage)"
	@echo "$(ramMessage)"
	@echo ""

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
# searches for all .d files in given directory and inserts them into the .c file
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)


######################################
# OpenOCD stuff
# TODO: add config.mk file for settings like programmer, etc.
######################################
CHIPSET ?= stm32f4x
FLASH_ADDRESS ?= 0x08000000

OCD=openocd
OCD_DIR ?= /usr/local/share/openocd/scripts # this value works, but for some reason this folder only exists at path -> /opt/homebrew/Cellar/open-ocd/0.11.0/share/openocd/scripts
PGM_DEVICE ?= interface/stlink.cfg
OCDFLAGS = -f $(PGM_DEVICE) -f target/$(CHIPSET).cfg

program:
	$(OCD) -s $(OCD_DIR) $(OCDFLAGS) \
		-c "program ./$(BUILD_DIR)/$(TARGET).elf verify reset exit"


DFU_INTERFACE_NUMBER = 0
DFU_ALT_SETTING = 0
DFU_FUSE_ADDRESS = $(FLASH_ADDRESS)

usb-upload:
	dfu-util -a $(DFU_ALT_SETTING) -s $(DFU_FUSE_ADDRESS):leave -D $(BUILD_DIR)/$(TARGET).bin
