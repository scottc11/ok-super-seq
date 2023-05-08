#include "SeqControl.h"

void SeqControl::init() {
    // initialize channel touch pads
    touch_pads->init();
    touch_pads->attachInterruptCallback(callback(this, &SeqControl::handleTouchInterrupt));
    touch_pads->attachCallbackTouched(callback(this, &SeqControl::onTouch));
    touch_pads->attachCallbackReleased(callback(this, &SeqControl::onRelease));
    touch_pads->enable();

    int_ppqn_1.rise(callback(this, &SeqControl::ppqn1_handler));
}

void SeqControl::ppqn1_handler() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t isr_id = ISR_ID_PPQN_1;
    xQueueSendFromISR(qh_interrupt_queue, &isr_id, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void SeqControl::advanceAll() {
    for (int i = 0; i < 4; i++)
    {
        channels[i]->advance();
    }
    
}

void SeqControl::handleTouchInterrupt()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t isr_id = ISR_ID_TOUCH;
    xQueueSendFromISR(qh_interrupt_queue, &isr_id, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void SeqControl::onTouch(uint8_t pad)
{
    
    if (pad < 8)
    {        
        int step = TOUCH_PAD_MAP[pad];
        for (int i = 0; i < NUM_CHANNELS; i++)
        {
            // prevent seq.advance from advancing anything
            channels[i]->override = true;
            channels[i]->handleTouchedStep(step);
        }
    }
    
}

void SeqControl::onRelease(uint8_t pad)
{
    if (pad < 8) {
        int step = TOUCH_PAD_MAP[pad];
        for (int i = 0; i < NUM_CHANNELS; i++)
        {
            if (touch_pads->padIsTouched()) {
                // if a pad is still touched, we want to snap back to the last touched pad
                // if more than 2 pads touched, how do we know which was the last touched?
                // you will need to create an array to hold all the curr touched pads, in the order they were touched
                // [4, 5, 6, 0, 0, 0, 0, 0]

                // channels[i]->activateStep(channels[i]->prevTouchedStep, channels[i]->currTouchedStep);
                
            } else {
                // snap back to currStep
                channels[i]->handleReleasedStep(step);
                channels[i]->override = false;
            }
        }
    }
}
const int SW1_POS1 = 0;
const int SW1_POS2 = 1;
const int SW1_POS3 = 2;
const int SW1_POS4 = 3;

void SeqControl::gpio_handler(uint16_t state)
{
    bool pin_state;
    for (int i = 0; i < 16; i++)
    {
        pin_state = bitwise_read_bit(state, i);
        if (pin_state == false) {
            switch (i)
            {
            case SW1_POS1:
                channels[0]->setDirection(Sequence::Direction::FORWARD);
                break;
            
            case SW1_POS2:
                channels[0]->setDirection(Sequence::Direction::BACKWARD);
                break;

            default:
                break;
            }
        }
    }
}