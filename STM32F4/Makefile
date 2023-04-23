######################################
# source
######################################
# C sources

C_SOURCES += \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
../STM32F4/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_exti.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
../STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
../STM32F4/middleware/FreeRTOS/Source/croutine.c \
../STM32F4/middleware/FreeRTOS/Source/event_groups.c \
../STM32F4/middleware/FreeRTOS/Source/list.c \
../STM32F4/middleware/FreeRTOS/Source/queue.c \
../STM32F4/middleware/FreeRTOS/Source/stream_buffer.c \
../STM32F4/middleware/FreeRTOS/Source/tasks.c \
../STM32F4/middleware/FreeRTOS/Source/timers.c \
../STM32F4/middleware/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c \
../STM32F4/middleware/FreeRTOS/Source/portable/MemMang/heap_4.c \
../STM32F4/middleware/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c \
../STM32F4/system/Src/freertos.c \
../STM32F4/system/Src/stm32f4xx_hal_timebase_tim.c \
../STM32F4/system/Src/stm32f4xx_hal_msp.c \
../STM32F4/system/Src/stm32f4xx_it.c \
../STM32F4/system/Src/system_stm32f4xx.c \
../STM32F4/system/Src/system_clock_config.c

CPP_SOURCES += \
../STM32F4/HAL/Src/gpio_api.cpp \
../STM32F4/HAL/Src/error_handler.cpp \
../STM32F4/HAL/Src/Flash.cpp \
../STM32F4/HAL/Src/I2C.cpp \
../STM32F4/HAL/Src/InterruptIn.cpp \
../STM32F4/HAL/Src/logger.cpp \
../STM32F4/HAL/Src/SPI.cpp \
../STM32F4/HAL/Src/DigitalIn.cpp \
../STM32F4/HAL/Src/DigitalOut.cpp \
../STM32F4/HAL/Src/SuperClock.cpp \
../STM32F4/HAL/Src/tim_api.cpp \
../STM32F4/HAL/rtos/Src/SoftwareTimer.cpp \
../STM32F4/HAL/rtos/Src/Mutex.cpp \


C_INCLUDES += \
-I../STM32F4/HAL/Inc \
-I../STM32F4/HAL/rtos/Inc \
-I../STM32F4/cxxsupport \
-I../STM32F4/STM32F4xx_HAL_Driver/Inc \
-I../STM32F4/STM32F4xx_HAL_Driver/Inc/Legacy \
-I../STM32F4/CMSIS/Device/ST/STM32F4xx/Include \
-I../STM32F4/CMSIS/Include \
-I../STM32F4/middleware/FreeRTOS/Source/include \
-I../STM32F4/middleware/FreeRTOS/Source/CMSIS_RTOS_V2 \
-I../STM32F4/middleware/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-I../STM32F4/system/Inc


# ASM sources ("Assembly Language") - defines main() function
ASM_SOURCES =  \
../STM32F4/startup_stm32f446xx.s

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = ../STM32F4/STM32F446RETx_FLASH.ld