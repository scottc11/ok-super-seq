#pragma once

#include "main.h"
#include "tasks.h"
#include "logger.h"
#include "GPIOExpanders.h"
#include "MCP23017.h"

extern MCP23017 gpio1;
extern MCP23017 gpio2;
extern MCP23017 gpio3;

void task_interrupt_handler(void *params);