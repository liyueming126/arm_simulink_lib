
#include "matrix.h"
#include <stdint.h>

void arm_float_to_q31(const float *pSrc, int32_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        double v = (double)pSrc[i] * 2147483648.0;
        if (v > 2147483647.0)
        {
            pDst[i] = 2147483647;
        }
        else if (v < -2147483648.0)
        {
            pDst[i] = (int32_t)(-2147483648);
        }
        else
        {
            int64_t iv = (int64_t)v;
            if (v < 0.0 && (double)iv != v) iv--;
            pDst[i] = (int32_t)iv;
        }
    }
}
