#include "task_interrupt_handler.h"

/**
 * @brief A task which listens to a queue of interrupt events
 *
 * @param params global control
 */
void task_interrupt_handler(void *params)
{
    SeqControl *controller = (SeqControl *)params;
    th_interrupt_handler = xTaskGetCurrentTaskHandle();
    qh_interrupt_queue = xQueueCreate(64, sizeof(uint8_t));
    logger_log_task_watermark();
    uint8_t isr_source;
    while (1)
    {
        xQueueReceive(qh_interrupt_queue, &isr_source, portMAX_DELAY);
        switch (isr_source)
        {
        case ISR_ID_TOUCH:
            controller->touch_pads->handleTouch();
            break;

        case ISR_ID_GPIO1:
            handle_gpio_interrupt(&gpio1);
            break;
        
        case ISR_ID_GPIO2:
            handle_gpio_interrupt(&gpio2);
            break;
        
        case ISR_ID_GPIO3:
            handle_gpio_interrupt(&gpio3);
            break;
        
        case ISR_ID_EXT_STEP:
            // controller->advanceAll();
            break;
        case ISR_ID_EXT_PULSE:
            for (int i = 0; i < NUM_CHANNELS; i++)
            {
                controller->channels[i]->callback_ppqn();
            }
            break;
        default:
            break;
        }
    }
}