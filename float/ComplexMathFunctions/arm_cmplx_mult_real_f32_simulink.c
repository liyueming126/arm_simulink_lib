#include <stdint.h>

typedef float float32_t;

void arm_cmplx_mult_real_f32(
  float32_t * pSrcCmplx,
  float32_t * pSrcReal,
  float32_t * pCmplxDst,
  uint32_t numSamples)
{
  float32_t in;

  while(numSamples > 0u)
  {
    in = *pSrcReal++;
    *pCmplxDst++ = (*pSrcCmplx++) * (in);
    *pCmplxDst++ = (*pSrcCmplx++) * (in);

    numSamples--;
  }
}