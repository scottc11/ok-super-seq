#include "Sequence.h"

int CHANNEL_LED_PINS[4][8] = {
    {15, 16, 17, 18, 19, 20, 21, 22},
    {11, 12, 13, 14, 23, 24, 25, 26},
    {10, 9, 8, 3, 34, 29, 28, 27},
    {7, 6, 5, 4, 33, 32, 31, 30}
};

void Sequence::init() {

}

void Sequence::setPlaybackMode(PlaybackMode mode) {
    pbMode = mode;
}

void Sequence::setDirection(Direction _direction) {
    direction = _direction;
}

void Sequence::advance() {
    prevStep = currStep;

    if (direction == FORWARD)
    {
        if (currStep == (length - 1))
        {
            currStep = 0;
        }
        else
        {
            currStep++;
        }
    }
    else
    {
        if (currStep == 0) {
            currStep = length - 1;
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
    mux.activateChannel(curr);
    setLED(curr, 127);
    setLED(prev, 0);
}

void Sequence::setLED(int step, int pwm)
{
    leds->setChannelPWM(CHANNEL_LED_PINS[index][step], pwm);
}