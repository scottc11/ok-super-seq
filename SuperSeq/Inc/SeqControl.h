#pragma once

#include "main.h"
#include "task_handles.h"
#include "MPR121.h"
#include "MCP23017.h"
#include "Sequence.h"
#include "GPIOExpanders.h"

class Sequence; // forward declaration

static const int TOUCH_PAD_MAP[12] = {7, 6, 5, 4, 3, 2, 1, 0, 3, 2, 1, 0};

class SeqControl
{
public:
    SeqControl(
        MPR121 *tp_ptr,
        Sequence *seq1_ptr,
        Sequence *seq2_ptr,
        Sequence *seq3_ptr,
        Sequence *seq4_ptr) : ext_step_int(TP_PPQN_1, PinMode::PullUp), ext_pulse_int(TP_PPQN_96, PinMode::PullUp), tp_reset_int(TP_RESET, PinMode::PullUp), masterClockOut(MASTER_CLOCK_OUT)
    {
        touch_pads = tp_ptr;
        channels[0] = seq1_ptr;
        channels[1] = seq2_ptr;
        channels[2] = seq3_ptr;
        channels[3] = seq4_ptr;
        globalPlayback = false;
    }

    InterruptIn ext_step_int;
    InterruptIn ext_pulse_int;
    InterruptIn tp_reset_int;

    DigitalOut masterClockOut;

    MPR121 *touch_pads;
    Sequence *channels[4];

    bool globalPlayback;

    int pulse;
    int position; // relative position in polyrhythm array
    int step;

    bool altPressed;
    bool alternateModeActive;
    bool settingLength;
    bool blinkState; // for blinking sequence LEDs when setting sequence length
    bool waitForClock;
    bool resetArmed; // when true, reset will be triggered on next step
    bool queue_stop; // start and stop actions only happen on next step
    bool queue_start;
    uint8_t selectedChannels;

    void init();
    
    void tpStepHandler();
    void tpPulseHandler();
    void tpPulseFallHandler();
    void tpResetHandler();
    
    void advanceAll();
    void resetAll();

    void setAltLED(bool state);
    void setRunLED(bool state);
    void blinkLEDs();

    void handleTouchInterrupt();
    void onTouch(uint8_t pad);
    void onRelease(uint8_t pad);

    void activateLengthMode(bool activate);
    void handleAltTouchPadFunctions(uint8_t pad);

    void handleEncoder(int channel, int bit_position, uint16_t pin_states);
    void handleEncoderButton(int channel, int state);
    void handleAltButtonPress(int state);
    void handleRunButtonPress(int state);
    void handleResetButtonPress(int state);
    void handleSlideSwitch(int channel, int position, int state);
    void handleClockSwitch(int targetIndex, int sourceIndex, int state, int other_pin);
    void handleModSwitch(int targetIndex, int sourceIndex, int state, int other_pin);
};