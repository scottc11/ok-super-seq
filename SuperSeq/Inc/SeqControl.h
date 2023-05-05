#pragma once

#include "main.h"
#include "tasks.h"
#include "MPR121.h"
#include "Sequence.h"

class SeqControl
{
public:
    SeqControl(MPR121 *tp_ptr, Sequence *seq1_ptr, Sequence *seq2_ptr, Sequence *seq3_ptr, Sequence *seq4_ptr) : int_ppqn_1(EXT_PPQN_1, PinMode::PullUp)
    {
        touch_pads = tp_ptr;
        sequences[0] = seq1_ptr;
        sequences[1] = seq2_ptr;
        sequences[2] = seq3_ptr;
        sequences[3] = seq4_ptr;
        _pad = 6;
        
    }

    InterruptIn int_ppqn_1;
    
    MPR121 *touch_pads;
    
    Sequence *sequences[4];

    bool touched;
    uint8_t _pad;

    void init();
    void ppqn1_handler();
    void advanceAll();

    void poll();
    void handleTouchInterrupt();
    void onTouch(uint8_t pad);
    void onRelease(uint8_t pad);
};