#pragma once

#include "main.h"
#include "CD4051.h"
#include "IS31FL3246.h"

class Sequence {
public:
    Sequence(int _index, IS31FL3246 *leds_ptr, PinName mux_sel_a, PinName mux_sel_b, PinName mux_sel_c) : mux(mux_sel_a, mux_sel_b, mux_sel_c)
    {
        leds = leds_ptr;
        index = _index;
    }

    int index;
    CD4051 mux;
    IS31FL3246 *leds;
    int led_pins[8];

    int currStep;
    int prevStep;

    void init();
    void advance();
};