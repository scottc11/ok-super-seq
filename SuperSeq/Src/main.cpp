#include "main.h"
#include "DigitalOut.h"

DigitalOut gate(PB_10);


int main(void)
{
    HAL_Init();
    SystemClock_Config();

    gate.write(1);

    while (1)
    {
        HAL_Delay(1000);
        gate.toggle();
    }
}