#pragma once

#include "main.h"
#include "tasks.h"
#include "InterruptIn.h"
#include "MCP23017.h"

void gpio1_init(MCP23017 *gpio);
void gpio2_init(MCP23017 *gpio);
void gpio3_init(MCP23017 *gpio);

void int_callback_gpio1();
void int_callback_gpio2();
void int_callback_gpio3();