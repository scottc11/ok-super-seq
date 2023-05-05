#include "main.h"
#include "tim_api.h"
#include "logger.h"
#include "Callback.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "I2C.h"
#include "IS31FL3246.h"
#include "MPR121.h"
#include "MCP23017.h"
#include "SX1509.h"
#include "SeqControl.h"

DigitalOut gate(PB_10);
DigitalOut mux_1_A(PC_0);
DigitalOut mux_1_B(PC_1);
DigitalOut mux_1_C(PC_2);

InterruptIn int_pin_gpio3(PA_2, PinMode::PullUp); // interupt pin for buttons MCP23017 io

I2C i2c3(I2C3_SDA, I2C3_SCL, I2C::Instance::I2C_3);

MCP23017 gpio1(&i2c3, 0x20);
MCP23017 gpio2(&i2c3, 0x24);
MCP23017 gpio3(&i2c3, 0x22);

SX1509 ledsA(&i2c3, SX1509_CHAN_A_ADDR);
SX1509 ledsB(&i2c3, SX1509_CHAN_B_ADDR);
SX1509 ledsC(&i2c3, SX1509_CHAN_C_ADDR);
SX1509 ledsD(&i2c3, SX1509_CHAN_D_ADDR);

IS31FL3246 led_driver(&i2c3);
IS31FL3246 led_driver2(&i2c3, IS31FL3246_ADDR_VCC);

MPR121 touch_pads(&i2c3, TOUCH_INT);

// SeqControl controls(&touch_pads);

uint8_t portB = 0b00010101;
uint8_t pwm = 127;
bool flip = false;
bool changed = false;

void gpoi3_init();
void SX1509_init(SX1509 *leds);
void strobe();

void int_callback_gpio3() {
    changed = true;
}

int main(void)
{
    HAL_Init();
    
    SystemClock_Config();

    HAL_Delay(100);

    i2c3.init();

    bool isActive = gpio3.isConnected();

    gate.write(1);

    led_driver.init();
    led_driver.setGlobalCurrent(100, 100, 100);
    led_driver.setControlRegister(false, false, IS31FL3246::PWM_Frequency::_64kHz, false);
    for (int i = 0; i <= 36; i++)
    {
        led_driver.setChannelPWM(i, 0);
    }
    
    led_driver2.init();
    led_driver2.setChannelPWM(6, 20);
    led_driver2.setChannelPWM(7, 20);
    led_driver2.setChannelPWM(8, 20);
    led_driver2.setChannelPWM(9, 20);

    SX1509_init(&ledsA);
    SX1509_init(&ledsB);
    SX1509_init(&ledsC);
    SX1509_init(&ledsD);

    // controls.init();
    gpoi3_init();
    gpio3.digitalReadAB();
    int_pin_gpio3.fall(callback(int_callback_gpio3));

    int counter = 0;
    int led_arr_a[8] = { 15, 16, 17, 18, 19, 20, 21, 22 };
    int led_arr_b[8] = {11, 12, 13, 14, 23, 24, 25, 26};
    int led_arr_c[8] = {15, 16, 17, 18, 19, 20, 21, 22};
    int led_arr_d[8] = {15, 16, 17, 18, 19, 20, 21, 22};

    while (1)
    {
        HAL_Delay(500);

        switch (counter)
        {
        case 0:
            led_driver.setChannelPWM(led_arr_a[0], pwm);
            led_driver.setChannelPWM(led_arr_b[0], pwm);
            led_driver.setChannelPWM(led_arr_a[7], 0);
            led_driver.setChannelPWM(led_arr_b[7], 0);
            mux_1_A.write(0);
            mux_1_B.write(0);
            mux_1_C.write(0);
            break;
        case 1:
            led_driver.setChannelPWM(led_arr_a[1], pwm);
            led_driver.setChannelPWM(led_arr_b[1], pwm);
            led_driver.setChannelPWM(led_arr_a[0], 0);
            led_driver.setChannelPWM(led_arr_b[0], 0);
            mux_1_A.write(1);
            mux_1_B.write(0);
            mux_1_C.write(0);
            break;
        case 2:
            led_driver.setChannelPWM(led_arr_a[2], pwm);
            led_driver.setChannelPWM(led_arr_b[2], pwm);
            led_driver.setChannelPWM(led_arr_a[1], 0);
            led_driver.setChannelPWM(led_arr_b[1], 0);
            mux_1_A.write(0);
            mux_1_B.write(1);
            mux_1_C.write(0);
            break;
        case 3:
            led_driver.setChannelPWM(led_arr_a[3], pwm);
            led_driver.setChannelPWM(led_arr_b[3], pwm);
            led_driver.setChannelPWM(led_arr_a[2], 0);
            led_driver.setChannelPWM(led_arr_b[2], 0);
            mux_1_A.write(1);
            mux_1_B.write(1);
            mux_1_C.write(0);
            break;
        case 4:
            led_driver.setChannelPWM(led_arr_a[4], pwm);
            led_driver.setChannelPWM(led_arr_b[4], pwm);
            led_driver.setChannelPWM(led_arr_a[3], 0);
            led_driver.setChannelPWM(led_arr_b[3], 0);
            mux_1_A.write(0);
            mux_1_B.write(0);
            mux_1_C.write(1);
            break;
        case 5:
            led_driver.setChannelPWM(led_arr_a[5], pwm);
            led_driver.setChannelPWM(led_arr_b[5], pwm);
            led_driver.setChannelPWM(led_arr_a[4], 0);
            led_driver.setChannelPWM(led_arr_b[4], 0);
            mux_1_A.write(1);
            mux_1_B.write(0);
            mux_1_C.write(1);
            break;
        case 6:
            led_driver.setChannelPWM(led_arr_a[6], pwm);
            led_driver.setChannelPWM(led_arr_b[6], pwm);
            led_driver.setChannelPWM(led_arr_a[5], 0);
            led_driver.setChannelPWM(led_arr_b[5], 0);
            mux_1_A.write(0);
            mux_1_B.write(1);
            mux_1_C.write(1);
            break;
        case 7:
            led_driver.setChannelPWM(led_arr_a[7], pwm);
            led_driver.setChannelPWM(led_arr_b[7], pwm);
            led_driver.setChannelPWM(led_arr_a[6], 0);
            led_driver.setChannelPWM(led_arr_b[6], 0);
            mux_1_A.write(1);
            mux_1_B.write(1);
            mux_1_C.write(1);
            break;
        default:
            break;
        }

        counter++;
        
        if (counter > 7) {
            counter = 0;
        }
    }
}

void SX1509_init(SX1509 *leds) {
    leds->init();

    for (int i = 0; i < 16; i++)
    {
        leds->ledConfig(i);
        leds->setPWM(i, 255);
        leds->digitalWrite(i, 0); // turn on?
    }
}

void SX1509_test() {

}

void strobe() {
    int prev = 0;
    for (int i = 1; i <= 36; i++)
    {
        if (i == 1) {
            prev = 36;
        } else {
            prev = i - 1;
        }
        led_driver.setChannelPWM(i, 100);
        led_driver.setChannelPWM(prev, 0);
        HAL_Delay(200);
    }
}


void gpoi3_init() {
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