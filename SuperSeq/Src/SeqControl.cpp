#include "SeqControl.h"

void SeqControl::init() {
    // initialize channel touch pads
    touch_pads->init();
    touch_pads->attachInterruptCallback(callback(this, &SeqControl::handleTouchInterrupt));
    touch_pads->attachCallbackTouched(callback(this, &SeqControl::onTouch));
    touch_pads->attachCallbackReleased(callback(this, &SeqControl::onRelease));
    touch_pads->enable();

    ext_step_int.rise(callback(this, &SeqControl::ext_step_handler));
    ext_pulse_int.rise(callback(this, &SeqControl::ext_pulse_handler));
}

void SeqControl::ext_step_handler() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t isr_id = ISR_ID_EXT_STEP;
    xQueueSendFromISR(qh_interrupt_queue, &isr_id, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void SeqControl::ext_pulse_handler()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t isr_id = ISR_ID_EXT_PULSE;
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

// first compare the new state to the prev state to see whats changed
void SeqControl::gpio_handler(int id, uint16_t state)
{
    bool pin_state;
    for (int i = 0; i < 16; i++)
    {
        pin_state = bitwise_read_bit(state, i);
        if (pin_state == false) {
            switch (id)
            {
            case 1:
                /* code */
                break;
            case 2:
                handleGPIO2(i, pin_state);
                break;
            case 3:
                /* code */
                break;
            }
        }
    }
}

void SeqControl::handleGPIO2(uint8_t pin, bool state)
{
    switch (pin)
    {
    case GPIO2::SW1_POS1:
        channels[0]->setPlaybackMode(Sequence::PlaybackMode::DEFAULT);
        break;

    case GPIO2::SW1_POS2:
        channels[0]->setPlaybackMode(Sequence::PlaybackMode::PINGPONG);
        break;

    case GPIO2::SW1_POS3:
        channels[0]->setPlaybackMode(Sequence::PlaybackMode::PEDAL);
        break;

    case GPIO2::SW1_POS4:
        channels[0]->setPlaybackMode(Sequence::PlaybackMode::TOUCH);
        break;

    case GPIO2::CS_A_UP:
        // channel 1 adds its clock to channel 0
        break;

    case GPIO2::CS_A_DOWN:
        // channel 0 adds its clock to channel 1
        break;

    case GPIO2::MS_A_UP:
        // channel 1 mods channel 0
        break;

    case GPIO2::MS_A_DOWN:
        // channel 0 mods channel 1
        break;

    case GPIO2::ENC2_BTN:
        break;

    case GPIO2::SW2_POS1:
        break;

    case GPIO2::SW2_POS2:
        break;

    case GPIO2::SW2_POS3:
        break;

    case GPIO2::SW2_POS4:
        break;

    case GPIO2::ENC1_A:
        break;

    case GPIO2::ENC1_B:
        break;

    case GPIO2::ENC1_BTN:
        break;
    }
    return;
}    