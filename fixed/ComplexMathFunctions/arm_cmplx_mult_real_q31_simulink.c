
#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

static q31_t clip_q63_to_q31(q63_t x)
{
    return ((q31_t)(x >> 32) != ((q31_t)x >> 31))
        ? (q31_t)(0x7FFFFFFF ^ ((q31_t)(x >> 63)))
        : (q31_t)x;
}

void arm_cmplx_mult_real_q31(q31_t *pSrcCmplx, q31_t *pSrcReal, q31_t *pCmplxDst, uint32_t numSamples)
{
    q31_t inA1;

    while (numSamples > 0u)
    {
        inA1 = *pSrcReal++;
        *pCmplxDst++ = (q31_t)clip_q63_to_q31(((q63_t)(*pSrcCmplx++) * (q63_t)inA1) >> 31);
        *pCmplxDst++ = (q31_t)clip_q63_to_q31(((q63_t)(*pSrcCmplx++) * (q63_t)inA1) >> 31);
        numSamples--;
    }
}
