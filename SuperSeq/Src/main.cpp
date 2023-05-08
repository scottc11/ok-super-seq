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
#include "GPIOExpanders.h"

DigitalOut gate(PB_10);

InterruptIn int_pin_gpio3(PA_2, PinMode::PullUp); // interupt pin for buttons MCP23017 io

I2C i2c3(I2C3_SDA, I2C3_SCL, I2C::Instance::I2C_3);

MCP23017 gpio1(&i2c3, 0x20);
MCP23017 gpio2(&i2c3, 0x24);
MCP23017 gpio3(&i2c3, 0x22);

IS31FL3246 led_driver(&i2c3);

MPR121 touch_pads(&i2c3, TOUCH_INT);

Sequence seq1(0, &led_driver, CLOCK_OUT_1, MUX_1_A, MUX_1_B, MUX_1_C);
Sequence seq2(1, &led_driver, CLOCK_OUT_2, MUX_2_A, MUX_2_B, MUX_2_C);
Sequence seq3(2, &led_driver, CLOCK_OUT_3, MUX_3_A, MUX_3_B, MUX_3_C);
Sequence seq4(3, &led_driver, CLOCK_OUT_4, MUX_4_A, MUX_4_B, MUX_4_C);

SeqControl controller(&touch_pads, &gpio2, &seq1, &seq2, &seq3, &seq4);

uint8_t portB = 0b00010101;
uint8_t pwm = 127;
bool flip = false;
bool changed = false;

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

    gpio1_init(&gpio1);
    gpio2_init(&gpio2);
    gpio3_init(&gpio3);

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