#pragma once

#include "main.h"
#include "tasks.h"
#include "InterruptIn.h"
#include "MCP23017.h"
#include "SeqControl.h"

class SeqControl; // forward declaration

extern SeqControl controller;

void gpio1_init(MCP23017 *gpio);
void gpio2_init(MCP23017 *gpio);
void gpio3_init(MCP23017 *gpio);

void int_callback_gpio1();
void int_callback_gpio2();
void int_callback_gpio3();

void handle_gpio_interrupt(MCP23017 *gpio);

void gpio1_handler(int pin, uint16_t pin_states);
void gpio2_handler(int pin, uint16_t pin_states);
void gpio3_handler(int pin, uint16_t pin_states);

enum GPIO2
{
    SW1_POS1,
    SW1_POS2,
    SW1_POS3,
    SW1_POS4,
    CS_A_UP,
    CS_A_DOWN,
    MS_A_UP,
    MS_A_DOWN,
    ENC2_BTN,
    SW2_POS1,
    SW2_POS2,
    SW2_POS3,
    SW2_POS4,
    ENC1_A,
    ENC1_B,
    ENC1_BTN
};