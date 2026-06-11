
#include "matrix.h"
#include <stdint.h>

void arm_fill_q31(int32_t value, int32_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        pDst[i] = value;
    }
}
