#pragma once

#include "common.h"

// High nibble = port number (0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, 7=H)
// Low nibble  = pin number
#define STM_PORT(X) (((uint32_t)(X) >> 4) & 0xF)
#define STM_PIN(X) ((uint32_t)(X)&0xF)

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    PA_0 = 0x00,
    PA_1 = 0x01,
    PA_2 = 0x02,
    PA_3 = 0x03,
    PA_4 = 0x04,
    PA_5 = 0x05,
    PA_6 = 0x06,
    PA_7 = 0x07,
    PA_8 = 0x08,
    PA_9 = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,
    PB_0 = 0x10,
    PB_1 = 0x11,
    PB_2 = 0x12,
    PB_3 = 0x13,
    PB_4 = 0x14,
    PB_5 = 0x15,
    PB_6 = 0x16,
    PB_7 = 0x17,
    PB_8 = 0x18,
    PB_9 = 0x19,
    PB_10 = 0x1A,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_15 = 0x1F,
    PC_0 = 0x20,
    PC_1 = 0x21,
    PC_2 = 0x22,
    PC_3 = 0x23,
    PC_4 = 0x24,
    PC_5 = 0x25,
    PC_6 = 0x26,
    PC_7 = 0x27,
    PC_8 = 0x28,
    PC_9 = 0x29,
    PC_10 = 0x2A,
    PC_11 = 0x2B,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,
    PD_2 = 0x32,
    PH_0 = 0x70,
    PH_1 = 0x71,

    /**** ADC internal channels ****/

    ADC_TEMP = 0xF0, // Internal pin virtual value
    ADC_VREF = 0xF1, // Internal pin virtual value
    ADC_VBAT = 0xF2, // Internal pin virtual value
    
    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum
{
    PortA = 0,
    PortB = 1,
    PortC = 2,
    PortD = 3,
    PortE = 4,
    PortF = 5,
    PortG = 6,
    PortH = 7,
    PortI = 8,
    PortJ = 9,
    PortK = 10
} PortName;

typedef enum
{
    PullNone = 0,
    PullUp = 1,
    PullDown = 2,
    OpenDrainPullUp = 3,
    OpenDrainNoPull = 4,
    OpenDrainPullDown = 5
} PinMode;

typedef enum
{
    IRQ_EVENT_RISE,
    IRQ_EVENT_FALL,
    IRQ_EVENT_RISE_FALL
} IrqEvent;

typedef enum
{
    FREQ_LOW = GPIO_SPEED_FREQ_LOW,
    FREQ_MEDIUM = GPIO_SPEED_FREQ_MEDIUM,
    FREQ_HIGH = GPIO_SPEED_FREQ_HIGH,
    FREQ_VERY_HIGH = GPIO_SPEED_FREQ_VERY_HIGH,
} GPIO_Speed;

/**
 * Array which maps the value returned from STM_PIN to the HAL GPIO addresses
*/
static const uint32_t gpio_pin_map[16] = {
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15};

typedef struct
{
    PinName pin;
    uint32_t channel;
} AnalogPin;

// why does static need to be used here? Build gives mutliple define errors without it 🤷‍♂️
static const AnalogPin ADC_PIN_MAP[16] = {
    {PA_0, ADC_CHANNEL_0},
    {PA_1, ADC_CHANNEL_1},
    {PA_2, ADC_CHANNEL_2},
    {PA_3, ADC_CHANNEL_3},
    {PA_4, ADC_CHANNEL_4},
    {PA_5, ADC_CHANNEL_5},
    {PA_6, ADC_CHANNEL_6},
    {PA_7, ADC_CHANNEL_7},
    {PB_0, ADC_CHANNEL_8},
    {PB_1, ADC_CHANNEL_9},
    {PC_0, ADC_CHANNEL_10},
    {PC_1, ADC_CHANNEL_11},
    {PC_2, ADC_CHANNEL_12},
    {PC_3, ADC_CHANNEL_13},
    {PC_4, ADC_CHANNEL_14},
    {PC_5, ADC_CHANNEL_15}};

#ifdef __cplusplus
}
#endif