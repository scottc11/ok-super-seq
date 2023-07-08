#pragma once

#include "main.h"
#include "DigitalOut.h"
#include "CD4051.h"
#include "IS31FL3246.h"

class Sequence {
public:

    enum PlaybackMode {
        DEFAULT,
        PINGPONG,
        PEDAL,
        TOUCH
    };

    enum Direction
    {
        BACKWARD,
        FORWARD
    };

    Sequence(
        int _index,
        IS31FL3246 *leds_ptr,
        PinName trig_pin,
        PinName mux_sel_a,
        PinName mux_sel_b,
        PinName mux_sel_c) : mux(mux_sel_a, mux_sel_b, mux_sel_c), clockOut(trig_pin)
    {
        leds = leds_ptr;
        index = _index;
        currTouchedStep = 0;
        prevTouchedStep = 0;
        length = 8;
        direction = Direction::FORWARD; // you actually want to read state of switch in init to determine this
    }

    int index;
    DigitalOut clockOut;
    CD4051 mux;
    IS31FL3246 *leds;
    int led_pins[8];

    bool playback;
    PlaybackMode pbMode;
    bool direction;

    int clockDivider;
    int clockMultiplier;
    int currPulse;
    int currStep;
    int prevStep;
    uint8_t length;      // how long the sequence is in steps (max 8, min 1)
    uint16_t stepLength; // how many pulses to a step (zero-indexed!)
    bool queueStepLength;
    int currTouchedStep;
    int prevTouchedStep;
    bool override;

    void init();
    void setPlaybackMode(PlaybackMode mode);
    void setDirection(Direction _direction);
    void setRhythm(int value);
    void syncRhythmWithMaster();
    void syncRhythmWithChannel(int divider, int mutiplier);

    void start();
    void stop();
    void advance();
    void reset();

    uint16_t calculateStepLength();
    void updateStepLength();
    void callback_ppqn();

    void handleDefaultMode();
    void handlePingPongMode();

    void handleTouchedStep(int step);
    void handleReleasedStep(int step);

    void activateStep(int curr, int prev);

    void setLED(int step, int pwm);
    void clearLEDs();
};