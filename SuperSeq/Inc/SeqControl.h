#pragma once

#include "main.h"
#include "MPR121.h"

class SeqControl
{
public:
    SeqControl(MPR121 *tp_ptr)
    {
        touch_pads = tp_ptr;
        _pad = 6;
    }

    MPR121 *touch_pads;
    bool touched;
    uint8_t _pad;

    void init()
    {
        // initialize channel touch pads
        touch_pads->init();
        // touch_pads->attachInterruptCallback(callback(this, &SeqControl::handleTouchInterrupt));
        touch_pads->attachCallbackTouched(callback(this, &SeqControl::onTouch));
        touch_pads->attachCallbackReleased(callback(this, &SeqControl::onRelease));
        touch_pads->enable();
    }

    void poll()
    {
        if (touch_pads->interruptDetected())
        {
            touch_pads->handleTouch();
            this->_pad += 1;
            touched = false;
        }
    }

    void handleTouchInterrupt()
    {
        touched = true;
    }

    void onTouch(uint8_t pad)
    {
        this->_pad = pad + 8;
    }

    void onRelease(uint8_t pad)
    {
    }
};