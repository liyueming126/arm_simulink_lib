
#include <stdint.h>

typedef int16_t q15_t;
typedef int64_t q63_t;

void arm_cmplx_mag_squared_q15(q15_t *pSrc, q15_t *pDst, uint32_t numSamples)
{
    q15_t real, imag;
    int32_t acc0, acc1;

    while (numSamples > 0u)
    {
        real = *pSrc++;
        imag = *pSrc++;
        acc0 = (int32_t)real * (int32_t)real;
        acc1 = (int32_t)imag * (int32_t)imag;
        *pDst++ = (q15_t)(((q63_t)acc0 + (q63_t)acc1) >> 17);
        numSamples--;
    }
}
