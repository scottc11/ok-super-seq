#include "Sequence.h"

const int PEDAL_PROGRESSION[16] = { 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 6 };

int CHANNEL_LED_PINS[4][8] = {
    {15, 16, 17, 18, 19, 20, 21, 22},
    {11, 12, 13, 14, 23, 24, 25, 26},
    {10, 9, 8, 3, 34, 29, 28, 27},
    {7, 6, 5, 4, 33, 32, 31, 30}
};

void Sequence::init() {
    adc.setFilter(0.1);
    playback = true;
    playback = true;
    setPlaybackMode(pbMode); // interrupts trigger this function on startup, so re-calling it ensure all the settings are correct for each mode;
}

void Sequence::setPlaybackMode(PlaybackMode mode) {
    pbMode = mode;
    playback = true;
    clearLEDs();
    switch (pbMode)
    {
    case PlaybackMode::DEFAULT:
        break;
    case PlaybackMode::PINGPONG:
        direction = FORWARD;
        break;
    case PlaybackMode::PEDAL:
        // an array of 16
        currPedalStep = 0;
        prevPedalStep = 0;
        break;
    case PlaybackMode::TOUCH:
        playback = false;
        activateStep(currStep, prevStep);
        break;
    }
}

void Sequence::setDirection(Direction _direction) {
    direction = _direction;
}

void Sequence::syncRhythmWithMaster()
{
    divisorIndex = POLYRHYTHM_DEFAULT_DIVISOR;
}

/**
 * @brief
 *
 * NOTE: Polythrythms and Polymeters are two very different things.
 * 
 * Polyrhythms: groups of different numbers of beats, but each group takes the same amount of time to return to "beat 1"
 * Polymeters:  two different time signiatures / meters, playing back at the same time. Each beat of a meter steps according to the tempo, but the phrases have different lengths
 *
 * TODO: because of this, you actually need to devide everything by 4? So (96 * 4) / diviser
 * 
 * @param value either 1 or -1
 */
void Sequence::setRhythm(int value) {
    divisorIndex += value;
    
    if (divisorIndex > POLYRHYTHM_NUM_DIVISORS) {
        divisorIndex = POLYRHYTHM_NUM_DIVISORS - 1;
    } else if (divisorIndex < 0) {
        divisorIndex = 0;
    }
}

/**
 * @brief triggers once for every PPQN
 *
 * @param pulse current out of POLYRHYTHM_PULSES
 */
void Sequence::handle_pulse(int pulse)
{
    if (currPulse != timeStamp)
    {
        clockOut.write(0); // always set the trig out back low
    }
    currPulse = pulse;

    if (bitwise_read_bit(POLYRHYTHMS[pulse], divisorIndex))
    {
        if (playback)
        {
            advance();
        }
    }
}

void Sequence::start() {
    playback = true;
}

void Sequence::stop() {
    playback = false;
}

void Sequence::advance() {
    timeStamp = currPulse;
    if (override == false && pbMode != PlaybackMode::TOUCH)
    {
        if (currStep == 0 && prevStep == 0) {
            prevStep = length - 1;
        }
        activateStep(currStep, prevStep);
    }

    if (clockTarget != NULL)
    {
        clockTarget->advance();
    }

    clockOut.write(1);

    prevStep = currStep;

    switch (pbMode)
    {
    case PlaybackMode::DEFAULT:
        handleDefaultMode();
        break;
    case PlaybackMode::PINGPONG:
        handlePingPongMode();
        break;
    case PlaybackMode::PEDAL:
        handlePedalMode();
        break;
    case PlaybackMode::TOUCH:
        /* code */
        break;
    }
}

void Sequence::reset() {
    currPulse = 0;
    prevStep = currStep;
    currStep = 0;
    currPedalStep = 0;
    clearLEDs();
    activateStep(currStep, prevStep);
}

void Sequence::handleDefaultMode() {
    if (currStep >= length - 1) {
        currStep = 0;
    } else {
        currStep++;
    }
}

void Sequence::handlePingPongMode() {
    if (direction == FORWARD)
    {
        if (currStep == (length - 1))
        {
            if (pbMode == PlaybackMode::PINGPONG)
            {
                direction = !direction;
            }
            else
            {
                currStep = 0;
            }
        }
        else
        {
            currStep++;
        }
    }
    else
    {
        if (currStep == 0)
        {

            if (pbMode == PlaybackMode::PINGPONG)
            {
                direction = !direction;
            }
            else
            {
                currStep = length - 1;
            }
        }
        else
        {
            currStep--;
        }
    }
}

void Sequence::handlePedalMode() {
    if (currPedalStep >= ((length * 2) - 1)) {
        currPedalStep = 0;
        currStep = PEDAL_PROGRESSION[currPedalStep];
    } else {
        currPedalStep++;
        currStep = PEDAL_PROGRESSION[currPedalStep];
    }
}

void Sequence::handleTouchedStep(int step)
{
    prevTouchedStep = currTouchedStep;
    currTouchedStep = step;
    clearLEDs();
    activateStep(currTouchedStep, currStep);
    if (!playback) {
        clockOut.write(1);
    }
    
}

void Sequence::handleReleasedStep(int step)
{
    if (pbMode != PlaybackMode::TOUCH) {
        activateStep(currStep, currTouchedStep);
    }
    if (!playback) {
        clockOut.write(0);
    }
}

void Sequence::activateStep(int curr, int prev) {
    mux.activateChannel(curr);
    setLED(prev, 0);
    setLED(curr, 127);
}

void Sequence::setLED(int step, int pwm)
{
    leds->setChannelPWM(CHANNEL_LED_PINS[index][step], pwm);
}

void Sequence::clearLEDs() {
    for (int i = 0; i < 8; i++)
    {
        setLED(i, 0);
    }
    
}

void Sequence::setClockTarget(Sequence *target)
{
    clockTarget = target;
}

void Sequence::clearClockTarget()
{
    clockTarget = nullptr;
}