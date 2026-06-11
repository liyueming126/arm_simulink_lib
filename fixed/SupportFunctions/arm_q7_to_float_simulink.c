
#include "matrix.h"
#include <stdint.h>

void arm_q7_to_float(const int8_t *pSrc, float *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        pDst[i] = (float)pSrc[i] / 128.0f;
    }
}
