
#include "matrix.h"
#include <stdint.h>

void arm_q15_to_q31(const int16_t *pSrc, int32_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        pDst[i] = (int32_t)pSrc[i] << 16;
    }
}
