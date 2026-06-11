
#include <stdint.h>

typedef int16_t q15_t;

void arm_cmplx_conj_q15(q15_t *pSrc, q15_t *pDst, uint32_t numSamples)
{
    while (numSamples > 0u)
    {
        *pDst++ = *pSrc++;
        q15_t in = *pSrc++;
        *pDst++ = (in == (q15_t)0x8000) ? (q15_t)0x7fff : (q15_t)(-in);
        numSamples--;
    }
}
