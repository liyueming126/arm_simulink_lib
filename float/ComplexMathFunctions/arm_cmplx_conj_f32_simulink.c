#include <stdint.h>

typedef float float32_t;

void arm_cmplx_conj_f32(
  float32_t * pSrc,
  float32_t * pDst,
  uint32_t numSamples)
{
  while(numSamples > 0u)
  {
    /* realOut + j (imagOut) = realIn + j (-1) imagIn */
    *pDst++ = *pSrc++;
    *pDst++ = -*pSrc++;

    /* Decrement the loop counter */
    numSamples--;
  }
}