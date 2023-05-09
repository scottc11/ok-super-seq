#pragma once

#include "main.h"
#include "tasks.h"
#include "MPR121.h"
#include "MCP23017.h"
#include "Sequence.h"

static const int TOUCH_PAD_MAP[12] = {7, 6, 5, 4, 3, 2, 1, 0, 3, 2, 1, 0};

class SeqControl
{
public:
    SeqControl(MPR121 *tp_ptr, MCP23017 *gpio2_ptr, Sequence *seq1_ptr, Sequence *seq2_ptr, Sequence *seq3_ptr, Sequence *seq4_ptr) : int_ppqn_1(EXT_PPQN_1, PinMode::PullUp)
    {
        touch_pads = tp_ptr;
        channels[0] = seq1_ptr;
        channels[1] = seq2_ptr;
        channels[2] = seq3_ptr;
        channels[3] = seq4_ptr;
        gpio2 = gpio2_ptr;
    }

    InterruptIn int_ppqn_1;
    MPR121 *touch_pads;
    MCP23017 *gpio1;
    MCP23017 *gpio2;
    MCP23017 *gpio3;
    Sequence *channels[4];

    void init();
    void ppqn1_handler();
    void advanceAll();

    void gpio_handler(int id, uint16_t state) __attribute__((optimize("O0")));

    void handleGPIO2(uint8_t pin, bool state);

    void handleTouchInterrupt();
    void onTouch(uint8_t pad);
    void onRelease(uint8_t pad);
};

enum GPIO1
{
    CS_B_UP,
    CS_B_DOWN,
    MS_B_UP,
    MS_B_DOWN,
    SW3_POS4,
    SW3_POS3,
    SW3_POS2,
    SW3_POS1,
    ENC4_BTN,
    ENC3_BTN,
    B4,
    B5,
    ENC2_A,
    ENC2_B,
};

enum GPIO2
{
    SW1_POS1,
    SW1_POS2,
    SW1_POS3,
    SW1_POS4,
    CS_A_UP,
    CS_A_DOWN,
    MS_A_UP,
    MS_A_DOWN,
    ENC2_BTN,
    SW2_POS1,
    SW2_POS2,
    SW2_POS3,
    SW2_POS4,
    ENC1_A,
    ENC1_B,
    ENC1_BTN
};

enum GPIO3
{
    MS_C_UP,
    MS_C_DOWN,
    CS_C_UP,
    CS_C_DOWN,
    SW4_POS4,
    SW4_POS3,
    SW4_POS2,
    SW4_POS1,
    RESET_LED,
    RESET_BTN,
    RUN_LED,
    RUN_BTN,
    ALT_LED,
    ALT_BTN,
    ENC4_A,
    ENC4_B
};