#include "SeqControl.h"

void SeqControl::init() {

    // initialize channel touch pads
    touch_pads->init();
    touch_pads->attachInterruptCallback(callback(this, &SeqControl::handleTouchInterrupt));
    touch_pads->attachCallbackTouched(callback(this, &SeqControl::onTouch));
    touch_pads->attachCallbackReleased(callback(this, &SeqControl::onRelease));
    touch_pads->enable();

    setRunLED(0);
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        channels[i]->init();
    }
    
    ext_step_int.rise(callback(this, &SeqControl::tpStepHandler));
    ext_pulse_int.rise(callback(this, &SeqControl::tpPulseHandler));
    ext_pulse_int.fall(callback(this, &SeqControl::tpPulseFallHandler));
    tp_reset_int.rise(callback(this, &SeqControl::tpResetHandler));
}

void SeqControl::tpStepHandler() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t isr_id = ISR_ID_TP_STEP;
    xQueueSendFromISR(qh_interrupt_queue, &isr_id, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void SeqControl::tpPulseHandler()
{
    dispatch_sequence_event_ISR(5, ACTION::PULSE);
}

void SeqControl::tpPulseFallHandler()
{
    dispatch_sequence_event_ISR(88, ACTION::ADVANCE);
}

void SeqControl::tpResetHandler() {
    dispatch_sequence_event_ISR(5, ACTION::RESET);
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
        position = pulse + ((PPQN * step));
        for (int i = 0; i < 4; i++)
        {
            channels[i]->handle_pulse(position);
        }
    }

    if (pulse < PPQN - 1) {
        pulse++;
    } else {
        pulse = 0;
        
        if (queue_stop) {
            queue_stop = false;
            dispatch_sequence_event(5, ACTION::STOP);
        }

        if (queue_start) {
            queue_start = false;
            dispatch_sequence_event(5, ACTION::START);
        }

        if (resetArmed) {
            dispatch_sequence_event(5, ACTION::RESET);
            resetArmed = false;
        }
        
        if (step < 15) {
            step++;
        } else {
            step = 0;
        }
    }
}

void SeqControl::resetAll() {
    pulse = 0;
    step = 0;
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        channels[i]->reset();
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
        
        // override default behavior when alt button is pressed, or alternate mode is active
        if (altPressed || alternateModeActive) {
            alternateModeActive = true;
            handleAltTouchPadFunctions(touchedStep);
            return;
        }

        for (int i = 0; i < NUM_CHANNELS; i++)
        {
            // touch-release needs to snap back to the most recently touched pad (that is still touched)
            
            // if a channel is selected, only apply the touch event to that channel
            if (selectedChannels != 0)
            {
                if (channels[i]->selected) {
                    channels[i]->override = true;
                    channels[i]->handleTouchedStep(touchedStep);
                }
            }
            else // if no channels are selected, apply the touch event to all channels
            {
                channels[i]->override = true;
                channels[i]->handleTouchedStep(touchedStep);
            }
        }
    } else {
        channels[TOUCH_PAD_MAP[pad]]->select(true);
        selectedChannels = bitwise_set_bit(selectedChannels, TOUCH_PAD_MAP[pad]);
        // maybe clear the touch pads linked list here?
    }
    
}

void SeqControl::onRelease(uint8_t pad)
{
    if (pad < 8) {
        int touchedStep = TOUCH_PAD_MAP[pad];

        // determine if another pad is still touched, and whether or not to activate that pad
        int lastTouchedStep = touch_pads->getLastTouchedNode();
        bool activateLastStep = false;
        if (lastTouchedStep != 0xFF && lastTouchedStep < 8) // only handle specific touch pads (8, 9, 10, 11 are 'select' pads)
        {
            activateLastStep = true;
            lastTouchedStep = TOUCH_PAD_MAP[lastTouchedStep];
        }

        for (int i = 0; i < NUM_CHANNELS; i++)
        {
            if (selectedChannels != 0) {
                if (channels[i]->selected) {
                    channels[i]->handleReleasedStep(touchedStep, lastTouchedStep, activateLastStep);
                } else {
                    channels[i]->override = false;
                }
            } else {
                channels[i]->handleReleasedStep(touchedStep, lastTouchedStep, activateLastStep);
            }
        }
    } else {
        channels[TOUCH_PAD_MAP[pad]]->select(false);
        selectedChannels = bitwise_clear_bit(selectedChannels, TOUCH_PAD_MAP[pad]);
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

    if (enc_chan_a_state == 0 && enc_chan_b_state == 1) // direction right
    {
        if (channels[channel]->settingLength)
        {
            channels[channel]->setLength( channels[channel]->length + 1 );
        }
        else
        {
            channels[channel]->setRhythm(channels[channel]->divisorIndex - 1, altPressed);
        }
    }
    else if (enc_chan_a_state == 0 && enc_chan_b_state == 0) // direction left
    {
        if (channels[channel]->settingLength)
        {
            channels[channel]->setLength( channels[channel]->length - 1 );
        }
        else
        {
            channels[channel]->setRhythm(channels[channel]->divisorIndex + 1, altPressed);
        }
    }
}

void SeqControl::handleEncoderButton(int channel, int state)
{
    if (state == LOW) {
        if (channels[channel]->temporaryRhythmAdjustments) {
            channels[channel]->savedDivisorIndex = channels[channel]->divisorIndex;
        } else {
            channels[channel]->syncRhythmWithMaster();
        }
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
    if (state == LOW) {
        if (alternateModeActive) {
            alternateModeActive = false;
            if (settingLength) {
                activateLengthMode(false);
            }
            return;
        }
        setAltLED(true);
        altPressed = true;
    } else {
        if (alternateModeActive) {
            setAltLED(true);
        } else {
            setAltLED(false);
            altPressed = false;
            
            for (int i = 0; i < 4; i++) // reset all channels to their saved divisors
            {
                if (channels[i]->temporaryRhythmAdjustments)
                {
                    channels[i]->temporaryRhythmAdjustments = false;
                    channels[i]->setRhythm(channels[i]->savedDivisorIndex);
                }
            }
        }
    }
}

void SeqControl::handleRunButtonPress(int state)
{
    if (state == LOW) {
        globalPlayback = !globalPlayback;
        if (globalPlayback) {
            queue_stop = true;
        } else {
            queue_start = true;
        }
    }
}

/**
 * @brief This function is called when the reset button is pressed
 *  It only performs a "soft" reset, meaning it only resets the sequence when the next step is reached
 */
void SeqControl::handleResetButtonPress(int state)
{
    if (state == LOW) {
        dispatch_sequence_event(5, ACTION::RESET_ARM);
    }
}

/**
 * @brief 
 */
void SeqControl::handleClockSwitch(int targetIndex, int sourceIndex, int state, int other_pin)
{
    if (state == LOW) {
        channels[targetIndex]->setClockTarget(channels[sourceIndex]);
    } else {
        if (state == HIGH && other_pin == HIGH)
        {
            // neutral position
            channels[targetIndex]->clearClockTarget();
            channels[sourceIndex]->clearClockTarget();
        }
    }
}

/**
 * @brief
 */
void SeqControl::handleModSwitch(int targetIndex, int sourceIndex, int state, int other_pin)
{
    if (state == LOW) {
        channels[targetIndex]->setModTarget(channels[sourceIndex]);
    } else {
        if (state == HIGH && other_pin == HIGH) {
            channels[targetIndex]->clearModTarget();
            channels[sourceIndex]->clearModTarget();
        }
    }
}

void SeqControl::setAltLED(bool state)
{
    int currState = gpio3.digitalRead(MCP23017_PORTB);
    currState = bitwise_write_bit(currState, GPIO3::ALT_LED - 8, state);
    gpio3.digitalWrite(MCP23017_PORTB, currState);
}

void SeqControl::setRunLED(bool state)
{
    int currState = gpio3.digitalRead(MCP23017_PORTB);
    currState = bitwise_write_bit(currState, GPIO3::RUN_LED - 8, state);
    gpio3.digitalWrite(MCP23017_PORTB, currState);
}

void SeqControl::activateLengthMode(bool activate)
{
    if (activate) {
        for (int i = 0; i < 4; i++)
        {
            channels[i]->settingLength = true;
            channels[i]->drawLength();
        }
    } else {
        for (int i = 0; i < 4; i++)
        {
            channels[i]->settingLength = false;
#ifdef PLAYBACK_SHOWS_SEQ_LENGTH
            channels[i]->drawLength(PWM_DISPLAY_LENGTH);
#endif
            channels[i]->activateStep(channels[i]->currStep, channels[i]->prevStep);
        }
    }
    settingLength = activate;
}

void SeqControl::blinkLEDs() {
    blinkState = !blinkState;
    for (int i = 0; i < 4; i++)
    {
        if (blinkState)
        {
            channels[i]->drawLength(PWM_SET_LENGTH);
        }
        else
        {
            channels[i]->drawLength(5);
        }
    }
}

void SeqControl::handleAltTouchPadFunctions(uint8_t pad)
{
    switch (pad)
    {
    case 0: // LENGTH
        activateLengthMode(true);
        break;
    default:
        break;
    }
}