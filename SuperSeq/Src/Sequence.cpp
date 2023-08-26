#include "Sequence.h"

const int PEDAL_PROGRESSION[16] = { 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 6 };

int CHANNEL_LED_PINS[4][8] = {
    {15, 16, 17, 18, 19, 20, 21, 22},
    {11, 12, 13, 14, 23, 24, 25, 26},
    {10, 9, 8, 3, 34, 29, 28, 27},
    {7, 6, 5, 4, 33, 32, 31, 30}
};

void Sequence::init() {
    adc.setFilter(0.99);
    playback = true;
    setPlaybackMode(pbMode); // interrupts trigger this function on startup, so re-calling it ensure all the settings are correct for each mode;
}

void Sequence::setPlaybackMode(PlaybackMode mode) {
    pbMode = mode;
    playback = true;
    clearLEDs();
    activateStep(currStep, prevStep);
    switch (pbMode)
    {
    case PlaybackMode::DEFAULT:
        direction = FORWARD;
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

void Sequence::setLength(int value) {
    if (value > 8 || value < 1) {
        return;
    }
    length = value;
    clearLEDs();
    drawLength();
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

    if (playback) {
        if (has_event(pulse))
        {
            timeStamp = currPulse;
            if (override == false && pbMode != PlaybackMode::TOUCH)
            {
                if (currStep == 0 && prevStep == 0)
                { // this should probably get moved to init?
                    prevStep = length - 1;
                }
                activateStep(currStep, prevStep);
                resetOccured = false;
            }
            clockOut.write(1);
        }
    }
}

/**
 * @brief The down pulse is when we read the ADC of a modification source, should there be one, as well
 * as advance the sequence if there is a trigger
 * 
 * @param pulse 
 */
void Sequence::handle_down_pulse(int pulse)
{
    handleModificationSource();
    if (playback) {
        for (int i = 0; i < num_triggers(pulse); i++)
        {
            advance();
        }
    }
}

int Sequence::num_triggers(int pulse) {
    int trigger_count = 0;
    if (bitwise_read_bit(POLYRHYTHMS[pulse], divisorIndex)) {
        trigger_count += 1;
    }
    if (clockModSource)
    {
        trigger_count += clockModSource->num_triggers(pulse);
    }
    return trigger_count;
}

/**
 * @brief Recursive function which checks if that channel, or any of its mod sources, has an event at the currPulse
 *
 */
bool Sequence::has_event(int pulse)
{
    if (bitwise_read_bit(POLYRHYTHMS[pulse], divisorIndex))
    {
        return true;
    }
    else if (clockModSource && clockModSource->has_event(pulse))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Sequence::start() {
    playback = true;
}

void Sequence::stop() {
    playback = false;
}

void Sequence::advance() {
    prevStep = currStep;

    switch (pbMode)
    {
    case PlaybackMode::DEFAULT:
        handlePingPongMode();
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
    activateStep(currStep, prevStep);
}

// this function needs to become the default mode function. Its already setup for it.
void Sequence::handlePingPongMode() {
    if (direction == FORWARD)
    {
        if (currStep >= (length - 1))
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
        if (currStep <= 0)
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
            if (currStep <= length - 1) {
                currStep--;
            } else {
                currStep = length - 1;
            }
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

/**
 * @brief main function for handling output of the sequence
 * 
 * @param curr 
 * @param prev 
 */
void Sequence::activateStep(int curr, int prev) {
    mux.activateChannel(curr);
    if (!settingLength) {
        setLED(lastStepIlluminated, 0);
        lastStepIlluminated = curr;
        setLED(curr, 127);
    }
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

void Sequence::drawLength()
{
    for (int i = 0; i < length; i++)
    {
        setLED(i, 20);
    }
}

void Sequence::setClockTarget(Sequence *target)
{
    clockModSource = target;
}

void Sequence::clearClockTarget()
{
    clockModSource = nullptr;
}

void Sequence::setModTarget(Sequence *target)
{
    playbackModSource = target;
}

void Sequence::clearModTarget()
{
    playbackModSource = nullptr;
    setPlaybackMode(pbMode); // ensures direction is accurate
}


void Sequence::handleModificationSource()
{
    if (playbackModSource != nullptr)
    {
        uint16_t val = playbackModSource->adc.read_u16();
        playback = true;
        if (val < BIT_MAX_16 / 4) // 16,383.75
        {
            this->setDirection(Direction::FORWARD);
        }
        else if (val < BIT_MAX_16 / 2) // 32,767.5
        {
            // reverse
            this->setDirection(Direction::BACKWARD);
        }
        else if (val < (BIT_MAX_16 / 2) + (BIT_MAX_16 / 4))
        {
            // reset is odd because once you come to a reset step, you just want to reset the
            // target sequence, and then have the target sequence continue progressing (otherwise this would be very similar to a 'freeze' event)
            // you need to set a flag on source sequence when a reset occurs, and then only
            // reset that flag when the source sequence progresses to its next step
            if (!playbackModSource->resetOccured) {
                this->reset();
                playbackModSource->resetOccured = true;
            }
        }
        else
        {
            // freeze stops both the sequence playback AND clock out
            playback = false;
        }
    }
}