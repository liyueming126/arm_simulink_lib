
#include "matrix.h"
#include <stdint.h>

void arm_float_to_q7(const float *pSrc, int8_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        float v = pSrc[i] * 128.0f;
        if (v > 127.0f)
        {
            pDst[i] = 127;
        }
        else if (v < -128.0f)
        {
            pDst[i] = -128;
        }
        else
        {
            int32_t iv = (int32_t)v;
            if (v < 0.0f && (float)iv != v) iv--;
            pDst[i] = (int8_t)iv;
        }
    }
}
