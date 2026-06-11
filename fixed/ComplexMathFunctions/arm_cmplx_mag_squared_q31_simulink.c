
#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_cmplx_mag_squared_q31(q31_t *pSrc, q31_t *pDst, uint32_t numSamples)
{
    q31_t real, imag;
    q31_t acc0, acc1;

    while (numSamples > 0u)
    {
        real = *pSrc++;
        imag = *pSrc++;
        acc0 = (q31_t)(((q63_t)real * (q63_t)real) >> 33);
        acc1 = (q31_t)(((q63_t)imag * (q63_t)imag) >> 33);
        *pDst++ = acc0 + acc1;
        numSamples--;
    }
}
