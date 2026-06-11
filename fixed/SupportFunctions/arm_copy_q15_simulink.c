
#include "matrix.h"
#include <stdint.h>

void arm_copy_q15(const int16_t *pSrc, int16_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        pDst[i] = pSrc[i];
    }
}
