#include "Polyrhythms.h"

int POLY_1_TARGETS[1];
int POLY_2_TARGETS[2];
int POLY_3_TARGETS[3];
int POLY_4_TARGETS[4];
int POLY_5_TARGETS[5];

PolyArray POLY_1;
PolyArray POLY_2;
PolyArray POLY_3;
PolyArray POLY_4;
PolyArray POLY_5;

PolyArray* POLYRHYTHMS[5];

void init_polyrhythms()
{
    POLY_1 = {1, POLY_1_TARGETS};
    POLY_2 = {2, POLY_2_TARGETS};
    POLY_3 = {3, POLY_3_TARGETS};
    POLY_4 = {4, POLY_4_TARGETS};
    POLY_5 = {5, POLY_5_TARGETS};
    calculate_polyrhythm(&POLY_1);
    calculate_polyrhythm(&POLY_2);
    calculate_polyrhythm(&POLY_3);
    calculate_polyrhythm(&POLY_4);
    calculate_polyrhythm(&POLY_5);
    POLYRHYTHMS[0] = &POLY_1;
    POLYRHYTHMS[1] = &POLY_2;
    POLYRHYTHMS[2] = &POLY_3;
    POLYRHYTHMS[3] = &POLY_4;
    POLYRHYTHMS[4] = &POLY_5;
}

void calculate_polyrhythm(PolyArray *polyrhythm)
{
    for (int i = 0; i < polyrhythm->length; i++)
    {
        polyrhythm->triggers[i] = ((PPQN * 4 / polyrhythm->length) * i);
    }
}