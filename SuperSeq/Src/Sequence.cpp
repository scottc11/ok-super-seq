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

    mux.activateChannel(currStep);
    leds->setChannelPWM(CHANNEL_LED_PINS[index][currStep], 127);
    leds->setChannelPWM(CHANNEL_LED_PINS[index][prevStep], 0);
}