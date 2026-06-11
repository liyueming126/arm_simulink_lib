
#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_cmplx_mult_cmplx_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t numSamples)
{
    q31_t a, b, c, d;
    q31_t mul1, mul2, mul3, mul4;

    while (numSamples > 0u)
    {
        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;

        mul1 = (q31_t)(((q63_t)a * c) >> 32);
        mul2 = (q31_t)(((q63_t)b * d) >> 32);
        mul3 = (q31_t)(((q63_t)a * d) >> 32);
        mul4 = (q31_t)(((q63_t)b * c) >> 32);
        mul1 = (mul1 >> 1);
        mul2 = (mul2 >> 1);
        mul3 = (mul3 >> 1);
        mul4 = (mul4 >> 1);

        *pDst++ = mul1 - mul2;
        *pDst++ = mul3 + mul4;

        numSamples--;
    }
}
