/* ----------------------------------------------------------------------
 * arm_q15_to_float_simulink.c
 * Simplified self-contained implementation of arm_q15_to_float for Simulink.
 * Converts Q15 (16-bit integer) vector to float.
 * Formula: pDst[n] = (float32_t) pSrc[n] / 32768
 * -------------------------------------------------------------------- */
#include <stdint.h>

void arm_q15_to_float(
  int16_t * pSrc,
  float * pDst,
  uint32_t blockSize)
{
  uint32_t i;
  for (i = 0u; i < blockSize; i++)
  {
    pDst[i] = ((float)pSrc[i]) / 32768.0f;
  }
}