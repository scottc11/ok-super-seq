#pragma once
#include "main.h"

#define POLYRHYTHM_NUM_DIVISORS 24
#define POLYRHYTHM_DEFAULT_DIVISOR 11
#define POLYRHYTHM_PULSES (PPQN * 16)

extern uint32_t POLYRHYTHMS[POLYRHYTHM_PULSES]; // 1536
extern int DIVISORS[POLYRHYTHM_NUM_DIVISORS];

void calculate_polyrhythms();