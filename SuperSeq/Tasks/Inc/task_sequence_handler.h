#pragma once

#include "main.h"
#include "SeqControl.h"

enum class ACTION
{
    PULSE,
    ADVANCE,
    RESET,
    RESET_ARM,
    MODIFY
};
typedef enum ACTION ACTION;

void task_sequence_handler(void *params);
void dispatch_sequence_event(uint8_t channel, ACTION action);
void dispatch_sequence_event_ISR(uint8_t channel, ACTION action);