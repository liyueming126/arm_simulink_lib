
#include "matrix.h"
#include <stdint.h>

void arm_q15_to_q7(const int16_t *pSrc, int8_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        pDst[i] = (int8_t)(pSrc[i] >> 8);
    }
}
