#include "Sequence.h"

int CHANNEL_LED_PINS[4][8] = {
    {15, 16, 17, 18, 19, 20, 21, 22},
    {11, 12, 13, 14, 23, 24, 25, 26},
    {10, 9, 8, 3, 34, 29, 28, 27},
    {7, 6, 5, 4, 33, 32, 31, 30}
};

void Sequence::init() {

}

void Sequence::advance() {
    prevStep = currStep;

    if (currStep + 1 > 7) {
        currStep = 0;
    } else {
        currStep++;
    }

    if (override == false) {
        activateStep(currStep, prevStep);
    }
}

void Sequence::handleTouchedStep(int step)
{
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

// |  0  ||  1  ||  T  ||  3  ||  P  ||  5  ||  C  ||  7  |

// |  0  ||  1  ||  2  ||  3  ||  4  ||  5  ||  6  ||  7  |

/**
 * if 
 * 
 */