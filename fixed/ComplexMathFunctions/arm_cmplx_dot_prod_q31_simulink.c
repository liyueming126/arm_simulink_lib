
#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_cmplx_dot_prod_q31(q31_t *pSrcA, q31_t *pSrcB, uint32_t numSamples, q63_t *realResult, q63_t *imagResult)
{
    q63_t real_sum = 0, imag_sum = 0;
    q31_t a0, b0, c0, d0;

    while (numSamples > 0u)
    {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += ((q63_t)a0 * (q63_t)c0) >> 14;
        imag_sum += ((q63_t)a0 * (q63_t)d0) >> 14;
        real_sum -= ((q63_t)b0 * (q63_t)d0) >> 14;
        imag_sum += ((q63_t)b0 * (q63_t)c0) >> 14;

        numSamples--;
    }

    *realResult = real_sum;
    *imagResult = imag_sum;
}
