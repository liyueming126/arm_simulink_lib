#include <stdint.h>
#include <math.h>

typedef float float32_t;

void arm_cmplx_mag_f32(
  float32_t * pSrc,
  float32_t * pDst,
  uint32_t numSamples)
{
  float32_t realIn, imagIn;

  while(numSamples > 0u)
  {
    realIn = *pSrc++;
    imagIn = *pSrc++;

    *pDst++ = sqrtf((realIn * realIn) + (imagIn * imagIn));

    numSamples--;
  }
}