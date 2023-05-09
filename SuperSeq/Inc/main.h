#pragma once

#include "cmsis_os.h"
#include "common.h"

#define NUM_CHANNELS 4

#define PPQN 96

#define I2C3_SDA PC_9
#define I2C3_SCL PA_8

#define TOUCH_INT PC_12

#define GPIO1_INT PA_0
#define GPIO2_INT PA_1
#define GPIO3_INT PA_2

#define EXT_PPQN_1 PC_10
#define EXT_PPQN_96 PC_11
#define EXT_RESET PC_15

#define MASTER_CLOCK_IN PA_3

#define CLOCK_OUT_1 PB_10
#define CLOCK_OUT_2 PB_12
#define CLOCK_OUT_3 PC_3
#define CLOCK_OUT_4 PA_15

#define MUX_1_A PC_0
#define MUX_1_B PC_1
#define MUX_1_C PC_2

#define MUX_2_A PB_2
#define MUX_2_B PB_1
#define MUX_2_C PB_0

#define MUX_3_A PC_8
#define MUX_3_B PC_7
#define MUX_3_C PC_6

#define MUX_4_A PB_15
#define MUX_4_B PB_14
#define MUX_4_C PB_13

enum ISR_ID
{
    ISR_ID_GPIO1,
    ISR_ID_GPIO2,
    ISR_ID_GPIO3,
    ISR_ID_EXT_STEP,
    ISR_ID_EXT_PULSE,
    ISR_ID_TOUCH
};