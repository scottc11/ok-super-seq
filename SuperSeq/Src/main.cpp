#include "main.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "I2C.h"
#include "IS31FL3246.h"
#include "MPR121.h"

DigitalOut gate(PB_10);

I2C i2c3(I2C3_SDA, I2C3_SCL, I2C::Instance::I2C_3);

IS31FL3246 led_driver(&i2c3);
MPR121 touch_pads(&i2c3, TOUCH_INT);

class SeqControl {
public:
    SeqControl(MPR121 *tp_ptr)
    {
        touch_pads = tp_ptr;
        _pad = 6;
    }

    MPR121 *touch_pads;
    bool touched;
    uint8_t _pad;

    void init() {
        // initialize channel touch pads
        touch_pads->init();
        // touch_pads->attachInterruptCallback(callback(this, &SeqControl::handleTouchInterrupt));
        touch_pads->attachCallbackTouched(callback(this, &SeqControl::onTouch));
        touch_pads->attachCallbackReleased(callback(this, &SeqControl::onRelease));
        touch_pads->enable();
    }

    void poll() {
        if (touch_pads->interruptDetected())
        {
            touch_pads->handleTouch();
            this->_pad += 1;
            touched = false;
        }
        
    }

    void handleTouchInterrupt() {
        touched = true;
    }

    void onTouch(uint8_t pad) {
        this->_pad = pad + 8;
    }

    void onRelease(uint8_t pad) {

    }
};

SeqControl controls(&touch_pads);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    HAL_Delay(100);

    i2c3.init();

    HAL_Delay(100);

    gate.write(1);

    led_driver.init();
    led_driver.setGlobalCurrent(100, 100, 100);
    led_driver.setControlRegister(false, false, IS31FL3246::PWM_Frequency::_64kHz, false);
    for (int i = 0; i <= 36; i++)
    {
        led_driver.setChannelPWM(i, 0);
    }
    
    controls.init();

    // touch_pads.init();


    uint8_t pwm = 0;
    bool flip = false;
    while (1)
    {

        controls.poll();
        led_driver.setChannelPWM(controls._pad, 127);

        // HAL_Delay(50);
        // for (int i = 0; i <= 36; i++)
        // {
        //     led_driver.setChannelPWM(i, pwm);
        // }
        // if (pwm == 0)
        // {
        //     flip = false;
        // } else if (pwm == 20) {
        //     flip = true;
        // }
        
        // if (flip)
        // {
        //     pwm -= 1;
        // } else {
        //     pwm += 1;
        // }
        
        
    }
}