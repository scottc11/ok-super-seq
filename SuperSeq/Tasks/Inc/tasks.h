#pragma once

#include "main.h"
#include "task_interrupt_handler.h"

extern TaskHandle_t th_main;
extern TaskHandle_t th_interrupt_handler;
extern QueueHandle_t qh_interrupt_queue;
