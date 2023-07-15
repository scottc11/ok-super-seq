#pragma once

#include "main.h"

typedef struct
{
    int length;
    int *triggers;
} PolyArray;

#define POLY_BAR_LENGTH (PPQN * 8)

extern int POLY_1_TARGETS[1];
extern int POLY_2_TARGETS[2];
extern int POLY_3_TARGETS[3];
extern int POLY_4_TARGETS[4];
extern int POLY_5_TARGETS[5];

extern PolyArray POLY_1;
extern PolyArray POLY_2;
extern PolyArray POLY_3;
extern PolyArray POLY_4;
extern PolyArray POLY_5;

extern PolyArray* POLYRHYTHMS[5];

void init_polyrhythms();
void calculate_polyrhythm(PolyArray *polyrhythm);