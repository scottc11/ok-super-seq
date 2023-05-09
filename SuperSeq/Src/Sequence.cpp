#include "Sequence.h"

int CHANNEL_LED_PINS[4][8] = {
    {15, 16, 17, 18, 19, 20, 21, 22},
    {11, 12, 13, 14, 23, 24, 25, 26},
    {10, 9, 8, 3, 34, 29, 28, 27},
    {7, 6, 5, 4, 33, 32, 31, 30}
};

void Sequence::init() {
    clockDivider = 1;
    clockMultiplier = 1;
}

void Sequence::setPlaybackMode(PlaybackMode mode) {
    pbMode = mode;
    switch (pbMode)
    {
    case PlaybackMode::DEFAULT:
        direction = FORWARD;
        break;
    case PlaybackMode::PINGPONG:
        break;
    case PlaybackMode::PEDAL:
        break;
    case PlaybackMode::TOUCH:
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

// make an "END OF STEP" event to add to the queue
// this will take 

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

uint16_t Sequence::calculateStepLength()
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
    trigOut.write(0); // always set the trig out back low 

    if (currPulse == 0) {
        advance();
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

void Sequence::advance() {
    prevStep = currStep;

    if (direction == FORWARD)
    {
        if (currStep == (length - 1))
        {
            if (pbMode == PlaybackMode::PINGPONG)
            {
                direction = !direction;
            } else {
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
        if (currStep == 0) {
            
            if (pbMode == PlaybackMode::PINGPONG)
            {
                direction = !direction;
            } else {
                currStep = length - 1;
            }
        } else {
            currStep--;
        }
    }

    if (override == false) {
        activateStep(currStep, prevStep);
    }
}

void Sequence::handleTouchedStep(int step)
{
    // compare the currTouched and prevTouched from MPR121
    // if 
    // for each touched pad add to touched array
    

    prevTouchedStep = currTouchedStep;
    currTouchedStep = step;

    if (currTouchedStep != currStep) {
        activateStep(currTouchedStep, currStep);
    } else {
        // trigger clock out?
    }
    
    if (currTouchedStep != prevTouchedStep)
    {
        setLED(prevTouchedStep, 0);
    }
    
}

void Sequence::handleReleasedStep(int step)
{
    if (step != currStep) {
        activateStep(currStep, currTouchedStep);
    }
}

void Sequence::activateStep(int curr, int prev) {
    if (curr != prev) {
        mux.activateChannel(curr);
        setLED(curr, 127);
        setLED(prev, 0);
    }

    trigOut.write(1);
}

void Sequence::setLED(int step, int pwm)
{
    leds->setChannelPWM(CHANNEL_LED_PINS[index][step], pwm);
}