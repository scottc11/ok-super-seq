#pragma once

#include "main.h"
#include "tasks.h"
#include "InterruptIn.h"
#include "MCP23017.h"

enum GPIO1_BitPositions
{
    SW1_POS1 = 0,
    SW1_POS2 = 1,
    SW1_POS3 = 2,
    SW1_POS4 = 3
};

void gpio1_init(MCP23017 *gpio);
void gpio2_init(MCP23017 *gpio);
void gpio3_init(MCP23017 *gpio);

void int_callback_gpio1();
void int_callback_gpio2();
void int_callback_gpio3();