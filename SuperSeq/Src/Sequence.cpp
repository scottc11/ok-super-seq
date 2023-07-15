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
    clockDivider = 1;
    clockMultiplier = 1;
    updateStepLength();
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
    clockDivider = 1;
    clockMultiplier = 1;
    updateStepLength();
}
void Sequence::syncRhythmWithChannel(int divider, int multiplier)
{
    clockDivider = divider;
    clockMultiplier = multiplier;
    updateStepLength();
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
    // if value negative and divider > 1, subtract 1 from divider, and don't touch multiplier
    if (value < 0 && clockDivider > 1) {
        clockDivider -= 1;
    }

    // if value negative and divider == 1, add 1 to multiplier, and don't touch divider
    else if (value < 0 && clockDivider == 1)
    {
        clockMultiplier += 1;
        if (clockMultiplier > 16)
        {
            clockMultiplier = 16;
        }
    }

    // if value positive and multiplier > 1, subtract 1 from multiplier, don't touch divider
    else if (value > 0 && clockMultiplier > 1) {
        clockMultiplier -= 1;
    }

    // if value positive and multiplier == 1, add 1 to devidier and don't touch multiplier
    else if (value > 0 && clockMultiplier == 1) {
        clockDivider += 1;
        if (clockDivider > 16) {
            clockDivider = 16;
        }
    }

    updateStepLength();
}

// 384 / 5 == 76.8
// rounds down to 76
// meaning pulse will be set to 0 at beat 380
// meaning you must use the diviser to determine if you are on the last beat of the bar (example, beat 5 in a 5/4 polyrythm)
// in which case, wait till
uint16_t
Sequence::calculateStepLength()
{
    return ((PPQN * clockDivider) / clockMultiplier) - 1;
}

/**
 * @brief either imedieatly updates the stepLength, or queues an update for
 * when the next step overflow event occurs
 * 
 */
void Sequence::updateStepLength()
{
    if (currPulse > calculateStepLength())
    {
        queueStepLength = true;
    }
    else
    {
        stepLength = calculateStepLength();
    }
}

void Sequence::callback_ppqn() {
    if (currPulse != timeStamp) {
        clockOut.write(0); // always set the trig out back low
    }

    if (currPulse == 0) {
        if (playback) {
            advance();
            if (clockTarget != NULL)
            {
                clockTarget->advance(); // i guess depending on the time this takes place, the 
            }
        }
    }

    if (currPulse < stepLength)
    {
        currPulse++;
    }
    else
    {
        currPulse = 0;
        if (queueStepLength) {
            stepLength = calculateStepLength();
            queueStepLength = false;
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
    clockTarget = NULL;
}