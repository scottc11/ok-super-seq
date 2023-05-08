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

    void gpio_handler(uint16_t state) __attribute__((optimize("O0")));

    void handleTouchInterrupt();
    void onTouch(uint8_t pad);
    void onRelease(uint8_t pad);
};