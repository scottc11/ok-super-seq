#pragma once

#include "main.h"
#include "DigitalOut.h"
#include "AnalogHandle.h"
#include "CD4051.h"
#include "IS31FL3246.h"
#include "Polyrhythms.h"
#include "task_handles.h"

#define PWM_SET_LENGTH 20
#define PWM_DISPLAY_LENGTH 3
#define PWM_PROGRESS 127

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
        PinName adc_pin,
        PinName trig_pin,
        PinName mux_sel_a,
        PinName mux_sel_b,
        PinName mux_sel_c) : adc(adc_pin), clockOut(trig_pin), mux(mux_sel_a, mux_sel_b, mux_sel_c), clockModSource(nullptr), playbackModSource(nullptr)
    {
        leds = leds_ptr;
        index = _index;
        currTouchedStep = 0;
        prevTouchedStep = 0;
        length = 8;
        direction = Direction::FORWARD; // you actually want to read state of switch in init to determine this
    }

    int index;
    AnalogHandle adc;
    DigitalOut clockOut;
    CD4051 mux;
    IS31FL3246 *leds;
    int led_pins[8];

    bool playback;
    PlaybackMode pbMode;
    bool direction;
    bool resetOccured; // flag for sequence mod reset logic
    bool settingLength;

    Sequence *clockModSource;
    Sequence *playbackModSource;
    int timeStamp;

    int divisorIndex;
    int savedDivisorIndex;
    bool temporaryRhythmAdjustments;

    int currPulse;
    int currStep;
    int prevStep;
    int currPedalStep;
    int prevPedalStep;
    uint8_t lastStepIlluminated; // index of the LED which was last turned ON
    uint8_t length;      // how long the sequence is in steps (max 8, min 1)
    
    int currTouchedStep;
    int prevTouchedStep;
    bool override;
    bool selected;

    void init();
    void select(bool select);

    void setPlaybackMode(PlaybackMode mode);
    void setDirection(Direction _direction);
    void setRhythm(int value, bool temporary = false);
    void setLength(int value);
    void syncRhythmWithMaster();

    void setClockTarget(Sequence *target);
    void clearClockTarget();
    void setModTarget(Sequence *target);
    void clearModTarget();
    void handleModificationSource();

    void start();
    void stop();
    void advance();
    void reset();

    void handle_pulse(int pulse);
    void handle_down_pulse(int pulse);
    bool has_event(int pulse);
    int num_triggers(int pulse);

    void handlePingPongMode();
    void handlePedalMode();

    void handleTouchedStep(int step);
    void handleReleasedStep(int step);

    void activateStep(int curr, int prev);

    void setLED(int step, int pwm);
    void clearLEDs();
    void drawLength(uint8_t pwm = PWM_SET_LENGTH);
};