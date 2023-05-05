#include "main.h"
#include "tasks.h"
#include "tim_api.h"
#include "logger.h"
#include "Callback.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "I2C.h"
#include "IS31FL3246.h"
#include "MPR121.h"
#include "MCP23017.h"
#include "CD4051.h"
#include "SX1509.h"
#include "SeqControl.h"
#include "Sequence.h"

DigitalOut gate(PB_10);

InterruptIn int_pin_gpio3(PA_2, PinMode::PullUp); // interupt pin for buttons MCP23017 io

I2C i2c3(I2C3_SDA, I2C3_SCL, I2C::Instance::I2C_3);



MCP23017 gpio1(&i2c3, 0x20);
MCP23017 gpio2(&i2c3, 0x24);
MCP23017 gpio3(&i2c3, 0x22);

IS31FL3246 led_driver(&i2c3);

MPR121 touch_pads(&i2c3, TOUCH_INT);

Sequence seq1(0, &led_driver, MUX_1_A, MUX_1_B, MUX_1_C);
Sequence seq2(1, &led_driver, MUX_2_A, MUX_2_B, MUX_2_C);
Sequence seq3(2, &led_driver, MUX_3_A, MUX_3_B, MUX_3_C);
Sequence seq4(3, &led_driver, MUX_4_A, MUX_4_B, MUX_4_C);

SeqControl controller(&touch_pads, &seq1, &seq2, &seq3, &seq4);

uint8_t portB = 0b00010101;
uint8_t pwm = 127;
bool flip = false;
bool changed = false;

void gpio1_init();
void gpio2_init();
void gpio3_init();

void int_callback_gpio3() {
    changed = true;
}

void task_main(void *pvParameters)
{
    i2c3.init();

    led_driver.init();
    led_driver.setGlobalCurrent(100, 100, 100);
    led_driver.setControlRegister(false, false, IS31FL3246::PWM_Frequency::_64kHz, false);
    for (int i = 0; i <= 36; i++)
    {
        led_driver.setChannelPWM(i, 0);
    }

    gpio3_init();
    gpio3.digitalReadAB();
    int_pin_gpio3.fall(callback(int_callback_gpio3));

    controller.init();

    while (1)
    {

    }
}

int main(void) {
    HAL_Init();

    SystemClock_Config();

    HAL_Delay(100);

    xTaskCreate(task_main, "task_main", 512, NULL, 1, &th_main);
    xTaskCreate(task_interrupt_handler, "ISR handler", RTOS_STACK_SIZE_MIN, &controller, RTOS_PRIORITY_HIGH + 1, NULL);

    vTaskStartScheduler();

    while(1) {

    }
}

void gpio1_init()
{
    gpio1.init();

    // A0 = CS B Up
    // A1 = CS B Down
    // A2 = MS B Up
    // A3 = MS B Down
    // A4 = SW3P4
    // A5 = SW3P3
    // A6 = SW3P2
    // A7 = SW3P1
    gpio1.setDirection(MCP23017_PORTA, 0b11111111); // 1 = input, 0 = output
    gpio1.setInterupt(MCP23017_PORTA, 0b11111111);  // 1 = interupt, 0 = not interupt
    gpio1.setPullUp(MCP23017_PORTA, 0xff);

    // B0 = ENC4 BTN
    // B1 = ENC3 BTN
    // B2 = ENC3 A
    // B3 = ENC3 B
    // B4 = 
    // B5 = 
    // B6 = ENC2 A
    // B7 = ENC2 B
    gpio1.setDirection(MCP23017_PORTB, 0b11111111); // 1 = input, 0 = output
    gpio1.setInterupt(MCP23017_PORTB, 0b11001111);  // 1 = interupt, 0 = not interupt
    gpio1.setPullUp(MCP23017_PORTB, 0xff);
}

void gpio2_init() {
    gpio2.init();

    // A0 = SW1P1
    // A1 = SW1P2
    // A2 = SW1P3
    // A3 = SW1P4
    // A4 = CS A Up
    // A5 = CS A Down
    // A6 = MS A Up
    // A7 = MS A Down
    gpio2.setDirection(MCP23017_PORTA, 0b11111111); // 1 = input, 0 = output
    gpio2.setInterupt(MCP23017_PORTA, 0b11111111);  // 1 = interupt, 0 = not interupt
    gpio2.setPullUp(MCP23017_PORTA, 0xff);

    // B0 = ENC2 BTN
    // B1 = SW2P1
    // B2 = SW2P2
    // B3 = SW2P3
    // B4 = SW2P4
    // B5 = ENC1 A
    // B6 = ENC1 B
    // B7 = ENC1 BTN
    gpio2.setDirection(MCP23017_PORTB, 0b11111111); // 1 = input, 0 = output
    gpio2.setInterupt(MCP23017_PORTB, 0b11111111);  // 1 = interupt, 0 = not interupt
    gpio2.setPullUp(MCP23017_PORTB, 0xff);
}

void gpio3_init() {
    gpio3.init();
    // a0 = mod source C up
    // A1 = mod source C down
    // A2 = clock source C up
    // A3 = clock source C down
    // A4 = SW4P3
    // A5 = SW4P2
    // A6 = SW4P1
    // A7 = SW4P0
    gpio3.setDirection(MCP23017_PORTA, 0b11111111); // 1 = input, 0 = output
    gpio3.setInterupt(MCP23017_PORTA, 0b11111111);  // 1 = interupt, 0 = not interupt
    gpio3.setPullUp(MCP23017_PORTA, 0xff);

    // B0 = Reset LED
    // B1 = Reset BTN
    // B2 = RUN LED
    // B3 = RUN BTN
    // B4 = ALT LED
    // B5 = ALT BTN
    // B6 = ENC4_CHAN_A
    // B7 = ENC4_CHAN_B
    gpio3.setDirection(MCP23017_PORTB, 0b11101010);
    gpio3.setInterupt(MCP23017_PORTB, 0b11101010);
    gpio3.setPullUp(MCP23017_PORTB, 0b11101010);

    gpio3.setInputPolarity(MCP23017_PORTA, 0x00);
    gpio3.setInputPolarity(MCP23017_PORTB, 0x00);
}