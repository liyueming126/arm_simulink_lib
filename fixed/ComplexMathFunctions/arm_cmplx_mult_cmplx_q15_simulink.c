
#include <stdint.h>

typedef int16_t q15_t;

void arm_cmplx_mult_cmplx_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t numSamples)
{
    q15_t a, b, c, d;

    while (numSamples > 0u)
    {
        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;

        *pDst++ = (q15_t)((int32_t)((((int32_t)a * (int32_t)c) >> 15) - (((int32_t)b * (int32_t)d) >> 15)));
        *pDst++ = (q15_t)((int32_t)((((int32_t)a * (int32_t)d) >> 15) + (((int32_t)b * (int32_t)c) >> 15)));

        numSamples--;
    }
}
