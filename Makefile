
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

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build
LIB_PATH = ok-STM32F4


CPP_SOURCES += $(shell find SuperSeq -name '*.cpp')
CPP_SOURCES += $(shell find $(LIB_PATH)/drivers/MPR121 -name '*.cpp')
CPP_SOURCES += $(shell find $(LIB_PATH)/drivers/IS31FL3246 -name '*.cpp')
CPP_SOURCES += $(shell find $(LIB_PATH)/drivers/MCP23017 -name '*.cpp')
CPP_SOURCES += $(shell find $(LIB_PATH)/drivers/CD4051 -name '*.cpp')

# C includes
C_INCLUDES += \
-ISuperSeq/Inc \
-ISuperSeq/Tasks/Inc \
-I$(LIB_PATH)/drivers/MPR121 \
-I$(LIB_PATH)/drivers/MCP23017 \
-I$(LIB_PATH)/drivers/CD4051 \
-I$(LIB_PATH)/drivers/IS31FL3246

include $(LIB_PATH)/Makefile