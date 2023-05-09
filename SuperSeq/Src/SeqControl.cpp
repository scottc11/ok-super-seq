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

    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        channels[i]->init();
    }
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

/**
 * @brief 
 * 
 * @param channel sequence index
 * @param bit_position position in the 16-bit pin states
 * @param pin_states 
 */
void SeqControl::handleEncoder(int channel, int bit_position, uint16_t pin_states)
{
    int enc_chan_a_state = bitwise_read_bit(pin_states, bit_position);
    int enc_chan_b_state = bitwise_read_bit(pin_states, bit_position + 1);

    if (enc_chan_a_state == 0 && enc_chan_b_state == 1)
    {
        // direction right
        channels[channel]->setRhythm(-1);
    }
    else if (enc_chan_a_state == 0 && enc_chan_b_state == 0)
    {
        // direction left
        channels[channel]->setRhythm(1);
    }
}

void SeqControl::handleEncoderPress(int channel)
{
    controller.channels[channel]->syncRhythmWithMaster();
}

/**
 * @brief 
 * 
 * @param channel sequence index
 * @param position 1, 2, 3, or 4
 */
void SeqControl::handleSlideSwitch(int channel, int position)
{
    switch (position)
    {
    case 1:
        channels[channel]->setPlaybackMode(Sequence::PlaybackMode::DEFAULT);
        break;
    case 2:
        channels[channel]->setPlaybackMode(Sequence::PlaybackMode::PINGPONG);
        break;
    case 3:
        channels[channel]->setPlaybackMode(Sequence::PlaybackMode::PEDAL);
        break;
    case 4:
        channels[channel]->setPlaybackMode(Sequence::PlaybackMode::TOUCH);
        break;
    }
}

void SeqControl::handleAltButtonPress()
{

}

void SeqControl::handleRecordButtonPress()
{

}

/**
 * @brief Reset all sequences
 * 
 */
void SeqControl::handleResetButtonPress()
{
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        channels[i]->reset();
    }
}

/**
 * @brief 
 * 
 * @param id 1, 2, or 3 == A, B, C
 */
void SeqControl::handleClockSwitch(int id)
{

}

/**
 * @brief
 *
 * @param id 1, 2, or 3 == A, B, C
 */
void SeqControl::handleModSwitch(int id)
{

}