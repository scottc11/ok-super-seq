#include "main.h"
#include "task_handles.h"
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
#include "SeqControl.h"
#include "Sequence.h"
#include "Polyrhythms.h"
#include "GPIOExpanders.h"

DigitalOut gate(PB_10);

I2C i2c3(I2C3_SDA, I2C3_SCL, I2C::Instance::I2C_3);

MCP23017 gpio1(&i2c3, 0x20);
MCP23017 gpio2(&i2c3, 0x24);
MCP23017 gpio3(&i2c3, 0x22);

IS31FL3246 led_driver(&i2c3);

MPR121 touch_pads(&i2c3, TOUCH_INT);

Sequence seq1(0, &led_driver, ADC_1, CLOCK_OUT_1, MUX_1_A, MUX_1_B, MUX_1_C);
Sequence seq2(1, &led_driver, ADC_2, CLOCK_OUT_2, MUX_2_A, MUX_2_B, MUX_2_C);
Sequence seq3(2, &led_driver, ADC_3, CLOCK_OUT_3, MUX_3_A, MUX_3_B, MUX_3_C);
Sequence seq4(3, &led_driver, ADC_4, CLOCK_OUT_4, MUX_4_A, MUX_4_B, MUX_4_C);

SeqControl controller(&touch_pads, &seq1, &seq2, &seq3, &seq4);

/*
TODO: https://youtu.be/f5rsu8IdN8A?t=447
The way polyrthyms are deivided, if played fast enough, eventually will create chords 🤯
*/
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

    HAL_Delay(10);

    calculate_polyrhythms();

    controller.init();

    handle_gpio_interrupt(&gpio1, true);
    handle_gpio_interrupt(&gpio2, true);
    handle_gpio_interrupt(&gpio3, true);

    while (1)
    {

    }
}

int main(void) {
    HAL_Init();

    SystemClock_Config();

    AnalogHandle::initialize();
    HAL_Delay(100);

    xTaskCreate(task_sequence_handler, "Seq handler", RTOS_STACK_SIZE_MAX, &controller, RTOS_PRIORITY_HIGH + 1, &th_sequence_handler);
    xTaskCreate(task_interrupt_handler, "ISR handler", RTOS_STACK_SIZE_MIN, &controller, RTOS_PRIORITY_HIGH + 1, &th_interrupt_handler);
    xTaskCreate(task_main, "task_main", 512, NULL, 1, &th_main);

    vTaskStartScheduler();

    while(1) {

    }
}