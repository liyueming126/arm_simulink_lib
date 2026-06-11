/* ----------------------------------------------------------------------
 * arm_bitreversal_simulink.c
 * Simplified self-contained implementation of arm_bitreversal_f32 for Simulink.
 * Performs in-place bit reversal for floating-point FFT data.
 * -------------------------------------------------------------------- */
#include <stdint.h>

void arm_bitreversal_f32(
  float * pSrc,
  uint16_t fftSize,
  uint16_t bitRevFactor,
  uint16_t * pBitRevTab)
{
  uint16_t fftLenBy2, fftLenBy2p1;
  uint16_t i, j;
  float in;

  j = 0u;
  fftLenBy2 = fftSize >> 1u;
  fftLenBy2p1 = (fftSize >> 1u) + 1u;

  for (i = 0u; i <= (fftLenBy2 - 2u); i += 2u)
  {
    if(i < j)
    {
      in = pSrc[2u * i];
      pSrc[2u * i] = pSrc[2u * j];
      pSrc[2u * j] = in;

      in = pSrc[(2u * i) + 1u];
      pSrc[(2u * i) + 1u] = pSrc[(2u * j) + 1u];
      pSrc[(2u * j) + 1u] = in;

      in = pSrc[2u * (i + fftLenBy2p1)];
      pSrc[2u * (i + fftLenBy2p1)] = pSrc[2u * (j + fftLenBy2p1)];
      pSrc[2u * (j + fftLenBy2p1)] = in;

      in = pSrc[(2u * (i + fftLenBy2p1)) + 1u];
      pSrc[(2u * (i + fftLenBy2p1)) + 1u] = pSrc[(2u * (j + fftLenBy2p1)) + 1u];
      pSrc[(2u * (j + fftLenBy2p1)) + 1u] = in;
    }

    in = pSrc[2u * (i + 1u)];
    pSrc[2u * (i + 1u)] = pSrc[2u * (j + fftLenBy2)];
    pSrc[2u * (j + fftLenBy2)] = in;

    in = pSrc[(2u * (i + 1u)) + 1u];
    pSrc[(2u * (i + 1u)) + 1u] = pSrc[(2u * (j + fftLenBy2)) + 1u];
    pSrc[(2u * (j + fftLenBy2)) + 1u] = in;

    j = *pBitRevTab;
    pBitRevTab += bitRevFactor;
  }
}