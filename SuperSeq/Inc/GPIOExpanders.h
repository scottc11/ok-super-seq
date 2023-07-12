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

void gpio1_handler(int pin, int state);
void gpio2_handler(int pin, int state);
void gpio3_handler(int pin, int state);

enum GPIO1
{
    CS_B_UP,
    CS_B_DOWN,
    MS_B_UP,
    MS_B_DOWN,
    SW3_POS4,
    SW3_POS3,
    SW3_POS2,
    SW3_POS1,
    ENC4_BTN,
    ENC3_BTN,
    ENC3_A,
    ENC3_B,
    B4,
    B5,
    ENC2_A,
    ENC2_B,
};

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

enum GPIO3
{
    MS_C_UP,
    MS_C_DOWN,
    CS_C_UP,
    CS_C_DOWN,
    SW4_POS4,
    SW4_POS3,
    SW4_POS2,
    SW4_POS1,
    RESET_LED,
    RESET_BTN,
    RUN_LED,
    RUN_BTN,
    ALT_LED,
    ALT_BTN,
    ENC4_A,
    ENC4_B
};