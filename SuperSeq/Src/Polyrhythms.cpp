#include "Polyrhythms.h"

uint32_t POLYRHYTHMS[POLYRHYTHM_PULSES]; // 1536
//                                                                                      ||  
int DIVISORS[POLYRHYTHM_NUM_DIVISORS] = {256, 128, 112, 96, 80, 64, 48, 32, 28, 24, 20, 16, 15, 14, 13, 12, 8, 7, 6, 5, 4, 3, 2, 1};

/**
 * @brief This function determines the precise position of each polyrhrthms trigger position across
 * a total of 16 clock steps (ie. PPQN * 16). It populates a pre-initialized array of 32-bit integers
 * 
 * Each bit in each 32-bit number in the array represents a boolean determining if a polyrhthym event occurs (at that pulse position)
 * 
 * @note: Since some of the polyrhythms create floats when they divide, you need to account for the
 * error (over 1536 pulses, this error gets quite large and messes with the rhythm).
 * The tempo clock doesn't use floats to count (it uses an int), you need to round any floats
 */
void calculate_polyrhythms()
{
    for (int i = 0; i < POLYRHYTHM_NUM_DIVISORS; i++)
    {
        float divisor = DIVISORS[i];    // first get the divisor (ex. 80 ie. ((96 * 16) / 80))
        float pulses = (float)(POLYRHYTHM_PULSES) / divisor; // then find the frequency (ie. every 19.2 pulses)
        for (int x = 0; x < divisor; x++)
        {
            int index = x * pulses;
            POLYRHYTHMS[index] = bitwise_set_bit(POLYRHYTHMS[index], i);
        }
    }
}



/*

division: 128  pulses: 12  remainder: 0  modulo: 0


division: 80  pulses: 19.2  remainder: 16  modulo: 16


division: 64  pulses: 24  remainder: 0  modulo: 0


division: 48  pulses: 32  remainder: 0  modulo: 0


division: 32  pulses: 48  remainder: 0  modulo: 0


division: 28  pulses: 54  remainder: 24  modulo: 24


division: 24  pulses: 64  remainder: 0  modulo: 0


division: 20  pulses: 76  remainder: 16  modulo: 16


division: 16  pulses: 96  remainder: 0  modulo: 0


division: 15  pulses: 102  remainder: 6  modulo: 6


division: 14  pulses: 109  remainder: 10  modulo: 10


division: 12  pulses: 128  remainder: 0  modulo: 0


division: 8  pulses: 192  remainder: 0  modulo: 0


division: 7  pulses: 219  remainder: 3  modulo: 3  // meh


division: 6  pulses: 256  remainder: 0  modulo: 0


division: 5  pulses: 307  remainder: 1  modulo: 1 // might be cool


division: 4  pulses: 384  remainder: 0  modulo: 0


division: 3  pulses: 512  remainder: 0  modulo: 0


division: 2  pulses: 768  remainder: 0  modulo: 0


division: 1  pulses: 1536  remainder: 0  modulo: 0


((96 * 16) / 20) * 5 = 384

*/