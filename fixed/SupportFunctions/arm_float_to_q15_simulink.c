
#include "matrix.h"
#include <stdint.h>

void arm_float_to_q15(const float *pSrc, int16_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        float v = pSrc[i] * 32768.0f;
        if (v > 32767.0f)
        {
            pDst[i] = 32767;
        }
        else if (v < -32768.0f)
        {
            pDst[i] = -32768;
        }
        else
        {
            int32_t iv = (int32_t)v;
            if (v < 0.0f && (float)iv != v) iv--;
            pDst[i] = (int16_t)iv;
        }
    }
}
