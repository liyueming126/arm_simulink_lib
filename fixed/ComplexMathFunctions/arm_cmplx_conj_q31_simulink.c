
#include <stdint.h>

typedef int32_t q31_t;

void arm_cmplx_conj_q31(q31_t *pSrc, q31_t *pDst, uint32_t numSamples)
{
    while (numSamples > 0u)
    {
        *pDst++ = *pSrc++;
        q31_t in = *pSrc++;
        *pDst++ = (in == (q31_t)INT32_MIN) ? (q31_t)INT32_MAX : (q31_t)(-in);
        numSamples--;
    }
}
