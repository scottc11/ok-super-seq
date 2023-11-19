#include "task_sequence_handler.h"

/**
 * @brief A task which handles playback events of step sequencers
 *
 * @param params global control
 */
void task_sequence_handler(void *params)
{
    SeqControl *controller = (SeqControl *)params;
    th_sequence_handler = xTaskGetCurrentTaskHandle();
    qh_sequence_queue = xQueueCreate(64, sizeof(uint32_t));
    logger_log_task_watermark();
    uint32_t event;
    while (1)
    {
        xQueueReceive(qh_sequence_queue, &event, portMAX_DELAY);
        int channel = (int)bitwise_slice(event, 8, 8);
        ACTION action = (ACTION)bitwise_slice(event, 0, 8);
        switch (action)
        {
        case ACTION::START:
            controller->setRunLED(0);
            for (int i = 0; i < NUM_CHANNELS; i++)
            {
                controller->channels[i]->start();
            }
            break;
        
        case ACTION::STOP:
            controller->setRunLED(1);
            for (int i = 0; i < NUM_CHANNELS; i++)
            {
                controller->channels[i]->stop();
            }
            break;

        case ACTION::PULSE:
            controller->advanceAll();
            break;
        case ACTION::ADVANCE:
            for (int i = 0; i < 4; i++)
            {
                controller->channels[i]->handle_down_pulse(controller->position);
            }
            break;
        case ACTION::MODIFY:
            break;
        case ACTION::RESET_ARM:
            controller->resetArmed = true;
            break;
        case ACTION::RESET:
            controller->resetAll();
            break;
        }
    }
}

void dispatch_sequence_event(uint8_t channel, ACTION action)
{
    // |   |   | chan | event |
    uint32_t event = ((uint8_t)channel << 8) | (uint8_t)action;
    xQueueSend(qh_sequence_queue, &event, portMAX_DELAY);
}

void dispatch_sequence_event_ISR(uint8_t channel, ACTION action)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // | chan | event | position |
    uint32_t event = ((uint8_t)channel << 8) | (uint8_t)action;
    xQueueSendFromISR(qh_sequence_queue, &event, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}