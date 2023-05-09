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
            logger_log("\n### Toggle Switch ISR ###\n");
            controller->gpio_handler(1, controller->gpio2->digitalReadAB());
            break;
        
        case ISR_ID_GPIO2:
            logger_log("\n### Tactile Buttons ISR ###\n");
            controller->gpio_handler(2, controller->gpio2->digitalReadAB());
            break;
        
        case ISR_ID_GPIO3:
            logger_log("\n### Touch Pads ISR ###\n");
            controller->gpio_handler(3, controller->gpio2->digitalReadAB());
            break;
        
        case ISR_ID_PPQN_1:
            controller->advanceAll();
            break;
        default:
            break;
        }
    }
}