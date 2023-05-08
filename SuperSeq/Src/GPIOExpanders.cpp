#include "GPIOExpanders.h"

InterruptIn gpio1_int(GPIO1_INT, PinMode::PullUp);
InterruptIn gpio2_int(GPIO2_INT, PinMode::PullUp);
InterruptIn gpio3_int(GPIO3_INT, PinMode::PullUp);

void gpio1_init(MCP23017 *gpio)
{
    gpio->init();

    // A0 = CS B Up
    // A1 = CS B Down
    // A2 = MS B Up
    // A3 = MS B Down
    // A4 = SW3P4
    // A5 = SW3P3
    // A6 = SW3P2
    // A7 = SW3P1
    gpio->setDirection(MCP23017_PORTA, 0b11111111); // 1 = input, 0 = output
    gpio->setInterupt(MCP23017_PORTA, 0b11111111);  // 1 = interupt, 0 = not interupt
    gpio->setPullUp(MCP23017_PORTA, 0xff);

    // B0 = ENC4 BTN
    // B1 = ENC3 BTN
    // B2 = ENC3 A
    // B3 = ENC3 B
    // B4 =
    // B5 =
    // B6 = ENC2 A
    // B7 = ENC2 B
    gpio->setDirection(MCP23017_PORTB, 0b11111111); // 1 = input, 0 = output
    gpio->setInterupt(MCP23017_PORTB, 0b11001111);  // 1 = interupt, 0 = not interupt
    gpio->setPullUp(MCP23017_PORTB, 0xff);

    gpio->digitalReadAB();
    gpio1_int.fall(callback(int_callback_gpio1));
}

void gpio2_init(MCP23017 *gpio)
{
    gpio->init();

    // A0 = SW1P1
    // A1 = SW1P2
    // A2 = SW1P3
    // A3 = SW1P4
    // A4 = CS A Up
    // A5 = CS A Down
    // A6 = MS A Up
    // A7 = MS A Down
    gpio->setDirection(MCP23017_PORTA, 0b11111111); // 1 = input, 0 = output
    gpio->setInterupt(MCP23017_PORTA, 0b11111111);  // 1 = interupt, 0 = not interupt
    gpio->setPullUp(MCP23017_PORTA, 0xff);

    // B0 = ENC2 BTN
    // B1 = SW2P1
    // B2 = SW2P2
    // B3 = SW2P3
    // B4 = SW2P4
    // B5 = ENC1 A
    // B6 = ENC1 B
    // B7 = ENC1 BTN
    gpio->setDirection(MCP23017_PORTB, 0b11111111); // 1 = input, 0 = output
    gpio->setInterupt(MCP23017_PORTB, 0b11111111);  // 1 = interupt, 0 = not interupt
    gpio->setPullUp(MCP23017_PORTB, 0xff);

    gpio->digitalReadAB();

    gpio2_int.fall(callback(int_callback_gpio2));
}

void gpio3_init(MCP23017 *gpio)
{
    gpio->init();
    // a0 = mod source C up
    // A1 = mod source C down
    // A2 = clock source C up
    // A3 = clock source C down
    // A4 = SW4P3
    // A5 = SW4P2
    // A6 = SW4P1
    // A7 = SW4P0
    gpio->setDirection(MCP23017_PORTA, 0b11111111); // 1 = input, 0 = output
    gpio->setInterupt(MCP23017_PORTA, 0b11111111);  // 1 = interupt, 0 = not interupt
    gpio->setPullUp(MCP23017_PORTA, 0xff);

    // B0 = Reset LED
    // B1 = Reset BTN
    // B2 = RUN LED
    // B3 = RUN BTN
    // B4 = ALT LED
    // B5 = ALT BTN
    // B6 = ENC4_CHAN_A
    // B7 = ENC4_CHAN_B
    gpio->setDirection(MCP23017_PORTB, 0b11101010);
    gpio->setInterupt(MCP23017_PORTB, 0b11101010);
    gpio->setPullUp(MCP23017_PORTB, 0b11101010);

    gpio->setInputPolarity(MCP23017_PORTA, 0x00);
    gpio->setInputPolarity(MCP23017_PORTB, 0x00);

    gpio->digitalReadAB();
    gpio3_int.fall(callback(int_callback_gpio3));
}

void int_callback_gpio1() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t isr_id = ISR_ID_GPIO1;
    xQueueSendFromISR(qh_interrupt_queue, &isr_id, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void int_callback_gpio2()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t isr_id = ISR_ID_GPIO2;
    xQueueSendFromISR(qh_interrupt_queue, &isr_id, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void int_callback_gpio3()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t isr_id = ISR_ID_GPIO3;
    xQueueSendFromISR(qh_interrupt_queue, &isr_id, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
