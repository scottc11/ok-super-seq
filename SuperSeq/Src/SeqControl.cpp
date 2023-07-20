#include "SeqControl.h"

void SeqControl::init() {

    // initialize channel touch pads
    touch_pads->init();
    touch_pads->attachInterruptCallback(callback(this, &SeqControl::handleTouchInterrupt));
    touch_pads->attachCallbackTouched(callback(this, &SeqControl::onTouch));
    touch_pads->attachCallbackReleased(callback(this, &SeqControl::onRelease));
    touch_pads->enable();

    setRunLED(0);

    ext_step_int.rise(callback(this, &SeqControl::tpStepHandler));
    ext_pulse_int.rise(callback(this, &SeqControl::tpPulseHandler));
    tp_reset_int.rise(callback(this, &SeqControl::tpResetHandler));

    

    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        channels[i]->init();
    }
}

void SeqControl::tpStepHandler() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t isr_id = ISR_ID_TP_STEP;
    xQueueSendFromISR(qh_interrupt_queue, &isr_id, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void SeqControl::tpPulseHandler()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t isr_id = ISR_ID_TP_PULSE;
    xQueueSendFromISR(qh_interrupt_queue, &isr_id, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void SeqControl::tpResetHandler() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t isr_id = ISR_ID_TP_RESET;
    xQueueSendFromISR(qh_interrupt_queue, &isr_id, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void SeqControl::advanceAll() {
    if (pulse == 0) {
        masterClockOut.write(1);
    }
    if (pulse == 12) {
        masterClockOut.write(0);
    }

    if (!waitForClock)
    {
        for (int i = 0; i < 4; i++)
        {
            // channels[i]->callback_ppqn();
            int mod_pulse = pulse + ((PPQN * step));
            channels[i]->handle_pulse(mod_pulse);
        }
    }

    if (pulse < PPQN - 1) {
        pulse++;
    } else {
        pulse = 0;
        if (step < 15) {
            step++;
        } else {
            step = 0;
        }
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
        int touchedStep = TOUCH_PAD_MAP[pad];
        for (int i = 0; i < NUM_CHANNELS; i++)
        {

            // set a flag which tells the encoders to only temprarily increase/decrease the tempo, so that
            // on release, the clock dividers get set back to what they were prior to the touch

            // when globalPlayback == false:
            // don't let touch snap back to the currStep
            // trigger clock outs

            // when channel touch pad is touched, these touch pads only effect that channel

            // touch-release needs to snap back to the most recently touched pad (thatis still touched)

            channels[i]->override = true;
            channels[i]->handleTouchedStep(touchedStep);
        }
    }
    
}

void SeqControl::onRelease(uint8_t pad)
{
    if (pad < 8) {
        int touchedStep = TOUCH_PAD_MAP[pad];
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
                channels[i]->handleReleasedStep(touchedStep);
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

void SeqControl::handleEncoderButton(int channel, int state)
{
    if (state == LOW) {
        channels[channel]->syncRhythmWithMaster();
    }
}

/**
 * @brief
 *
 * @param channel sequence index
 * @param position 1, 2, 3, or 4
 * @param state pin state HIGH or LOW
 */
void SeqControl::handleSlideSwitch(int channel, int position, int state)
{
    if (state == LOW) {
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
}

void SeqControl::handleAltButtonPress(int state)
{

}

void SeqControl::handleRunButtonPress(int state)
{
    if (state == LOW) {
        globalPlayback = !globalPlayback;
        for (int i = 0; i < NUM_CHANNELS; i++)
        {
            if (globalPlayback)
            {
                setRunLED(1);
                channels[i]->stop();
            }
            else
            {
                setRunLED(0);
                channels[i]->start();
            }
        }
    }
}

/**
 * @brief Reset all sequences
 * 
 */
void SeqControl::handleResetButtonPress(int state)
{
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        if (state == LOW) {
            channels[i]->reset();
        }
    }
}

/**
 * @brief 
 * 
 * @param id 1, 2, or 3 == A, B, C
 */
void SeqControl::handleClockSwitch(int masterIndex, int slaveIndex, int state, int other_pin)
{
    if (state == LOW) {
        channels[masterIndex]->setClockTarget(controller.channels[slaveIndex]);
    } else {
        if (state == HIGH && other_pin == HIGH)
        {
            // neutral position
            channels[masterIndex]->clearClockTarget();
            channels[slaveIndex]->clearClockTarget();
        }
    }
}

/**
 * @brief
 *
 * @param id 1, 2, or 3 == A, B, C
 */
void SeqControl::handleModSwitch(int id)
{

}

void SeqControl::setRunLED(bool state)
{
    int currState = gpio3.digitalRead(MCP23017_PORTB);
    currState = bitwise_write_bit(currState, GPIO3::RUN_LED - 8, state);
    gpio3.digitalWrite(MCP23017_PORTB, currState);
}