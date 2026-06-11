#include <stdint.h>

typedef float float32_t;

void arm_cmplx_mag_squared_f32(
  float32_t * pSrc,
  float32_t * pDst,
  uint32_t numSamples)
{
  float32_t real, imag;

  while(numSamples > 0u)
  {
    real = *pSrc++;
    imag = *pSrc++;

    *pDst++ = (real * real) + (imag * imag);

    numSamples--;
  }
}