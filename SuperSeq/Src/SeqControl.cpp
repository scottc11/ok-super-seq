#include "SeqControl.h"


void SeqControl::init() {
    // initialize channel touch pads
    touch_pads->init();
    // touch_pads->attachInterruptCallback(callback(this, &SeqControl::handleTouchInterrupt));
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
        sequences[i]->advance();
    }
    
}

void SeqControl::poll()
{
    if (touch_pads->interruptDetected())
    {
        touch_pads->handleTouch();
        this->_pad += 1;
        touched = false;
    }
}

void SeqControl::handleTouchInterrupt()
{
    touched = true;
}

void SeqControl::onTouch(uint8_t pad)
{
    this->_pad = pad + 8;
}

void SeqControl::onRelease(uint8_t pad)
{
    
}