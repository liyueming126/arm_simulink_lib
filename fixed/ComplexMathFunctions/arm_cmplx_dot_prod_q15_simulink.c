
#include <stdint.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_cmplx_dot_prod_q15(q15_t *pSrcA, q15_t *pSrcB, uint32_t numSamples, q31_t *realResult, q31_t *imagResult)
{
    q63_t real_sum = 0, imag_sum = 0;
    q15_t a0, b0, c0, d0;

    while (numSamples > 0u)
    {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += (q31_t)a0 * (q31_t)c0;
        imag_sum += (q31_t)a0 * (q31_t)d0;
        real_sum -= (q31_t)b0 * (q31_t)d0;
        imag_sum += (q31_t)b0 * (q31_t)c0;

        numSamples--;
    }

    *realResult = (q31_t)(real_sum >> 6);
    *imagResult = (q31_t)(imag_sum >> 6);
}
