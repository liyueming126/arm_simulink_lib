#include <stdint.h>

typedef float float32_t;

void arm_cmplx_mult_cmplx_f32(
  float32_t * pSrcA,
  float32_t * pSrcB,
  float32_t * pDst,
  uint32_t numSamples)
{
  float32_t a1, b1, c1, d1;

  while(numSamples > 0u)
  {
    a1 = *pSrcA++;
    b1 = *pSrcA++;
    c1 = *pSrcB++;
    d1 = *pSrcB++;

    *pDst++ = (a1 * c1) - (b1 * d1);
    *pDst++ = (a1 * d1) + (b1 * c1);

    numSamples--;
  }
}