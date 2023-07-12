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
    gpio->setInterupt(MCP23017_PORTB, 0b01000111);  // 1 = interupt, 0 = not interupt
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
    gpio->setInterupt(MCP23017_PORTB, 0b10111111);  // 1 = interupt, 0 = not interupt
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
    gpio->setInterupt(MCP23017_PORTB, 0b01101010);
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

/**
 * @brief take a GPIO instance and funnel it to the correct handler
 * 
 * @param gpio pointer to gpio instance
 */
void handle_gpio_interrupt(MCP23017 *gpio) {
    gpio->digitalReadAB();
    uint8_t gpio_id = gpio->address >> 1;
    for (int i = 0; i < 16; i++)
    {
        if (bitwise_read_bit(gpio->currPinStates, i) != bitwise_read_bit(gpio->prevPinStates, i))
        {
            int pin_state = bitwise_read_bit(gpio->currPinStates, i);
            switch (gpio_id)
            {
            case GPIO1_I2C_ADDRESS:
                gpio1_handler(i, pin_state);
                break;
            case GPIO2_I2C_ADDRESS:
                gpio2_handler(i, pin_state);
                break;
            case GPIO3_I2C_ADDRESS:
                gpio3_handler(i, pin_state);
                break;
            }
        }
    }
}

void gpio1_handler(int pin, int state)
{
    switch (pin)
    {
    case GPIO1::CS_B_UP:
        
        break;

    case GPIO1::CS_B_DOWN:
        
        break;

    case GPIO1::MS_B_UP:
        
        break;

    case GPIO1::MS_B_DOWN:
        
        break;

    case GPIO1::SW3_POS4:
        controller.handleSlideSwitch(2, 4, state);
        break;

    case GPIO1::SW3_POS3:
        controller.handleSlideSwitch(2, 3, state);
        break;

    case GPIO1::SW3_POS2:
        controller.handleSlideSwitch(2, 2, state);
        break;

    case GPIO1::SW3_POS1:
        controller.handleSlideSwitch(2, 1, state);
        break;

    case GPIO1::ENC4_BTN:
        controller.handleEncoderButton(3, state);
        break;

    case GPIO1::ENC3_BTN:
        controller.handleEncoderButton(2, state);
        break;

    case GPIO1::ENC3_A:
        controller.handleEncoder(2, GPIO1::ENC3_A, gpio1.currPinStates);
        break;

    case GPIO1::ENC3_B:
        break;

    case GPIO1::B4:
        break;

    case GPIO1::B5:
        break;

    case GPIO1::ENC2_A:
        controller.handleEncoder(1, GPIO1::ENC2_A, gpio1.currPinStates);
        break;

    case GPIO1::ENC2_B:
        // do nothing...
        break;
    }
}

void gpio2_handler(int pin, int state)
{
    switch (pin)
    {
    case GPIO2::SW1_POS1:
        controller.handleSlideSwitch(0, 1, state);
        break;

    case GPIO2::SW1_POS2:
        controller.handleSlideSwitch(0, 2, state);
        break;

    case GPIO2::SW1_POS3:
        controller.handleSlideSwitch(0, 3, state);
        break;

    case GPIO2::SW1_POS4:
        controller.handleSlideSwitch(0, 4, state);
        break;

    case GPIO2::CS_A_UP:
        controller.handleClockSwitch(0, 1, state, bitwise_read_bit(gpio2.currPinStates, CS_A_DOWN));
        break;

    case GPIO2::CS_A_DOWN:
        controller.handleClockSwitch(1, 0, state, bitwise_read_bit(gpio2.currPinStates, CS_A_UP));
        break;

    case GPIO2::MS_A_UP:
        // channel 1 mods channel 0
        break;

    case GPIO2::MS_A_DOWN:
        // channel 0 mods channel 1
        break;

    case GPIO2::ENC2_BTN:
        controller.handleEncoderButton(1, state);
        break;

    case GPIO2::SW2_POS1:
        controller.handleSlideSwitch(1, 1, state);
        break;

    case GPIO2::SW2_POS2:
        controller.handleSlideSwitch(1, 2, state);
        break;

    case GPIO2::SW2_POS3:
        controller.handleSlideSwitch(1, 3, state);
        break;

    case GPIO2::SW2_POS4:
        controller.handleSlideSwitch(1, 4, state);
        break;

    case GPIO2::ENC1_A:
        controller.handleEncoder(0, GPIO2::ENC1_A, gpio2.currPinStates);
        break;

    case GPIO2::ENC1_B:
        // do nothing...
        break;

    case GPIO2::ENC1_BTN:
        controller.handleEncoderButton(0, state);
        break;
    }
}

void gpio3_handler(int pin, int state)
{
    switch (pin)
    {
    case GPIO3::MS_C_UP:
        
        break;

    case GPIO3::MS_C_DOWN:
        
        break;

    case GPIO3::CS_C_UP:
        
        break;

    case GPIO3::CS_C_DOWN:
        
        break;

    case GPIO3::SW4_POS4:
        controller.handleSlideSwitch(3, 4, state);
        break;

    case GPIO3::SW4_POS3:
        controller.handleSlideSwitch(3, 3, state);
        break;

    case GPIO3::SW4_POS2:
        controller.handleSlideSwitch(3, 2, state);
        break;

    case GPIO3::SW4_POS1:
        controller.handleSlideSwitch(3, 1, state);
        break;

    case GPIO3::RESET_LED:
        break;

    case GPIO3::RESET_BTN:
        controller.handleResetButtonPress(state);
        break;

    case GPIO3::RUN_LED:
        break;

    case GPIO3::RUN_BTN:
        controller.handleRunButtonPress(state);
        break;

    case GPIO3::ALT_LED:
        break;

    case GPIO3::ALT_BTN:
        controller.handleAltButtonPress(state);
        break;

    case GPIO3::ENC4_A:
        controller.handleEncoder(3, GPIO3::ENC4_A, gpio3.currPinStates);
        break;

    case GPIO3::ENC4_B:
        break;
    }
}