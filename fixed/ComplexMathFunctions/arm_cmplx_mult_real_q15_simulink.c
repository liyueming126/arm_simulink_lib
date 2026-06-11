
#include <stdint.h>

typedef int16_t q15_t;

static q15_t ssat_q15(int32_t val)
{
    if (val > 32767) return (q15_t)32767;
    if (val < -32768) return (q15_t)-32768;
    return (q15_t)val;
}

void arm_cmplx_mult_real_q15(q15_t *pSrcCmplx, q15_t *pSrcReal, q15_t *pCmplxDst, uint32_t numSamples)
{
    q15_t in;

    while (numSamples > 0u)
    {
        in = *pSrcReal++;
        *pCmplxDst++ = ssat_q15(((int32_t)(*pSrcCmplx++) * (int32_t)in) >> 15);
        *pCmplxDst++ = ssat_q15(((int32_t)(*pSrcCmplx++) * (int32_t)in) >> 15);
        numSamples--;
    }
}
