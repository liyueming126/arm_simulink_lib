/* ----------------------------------------------------------------------
 * arm_q31_to_float_simulink.c
 * Simplified self-contained implementation of arm_q31_to_float for Simulink.
 * Converts Q31 (32-bit integer) vector to float.
 * Formula: pDst[n] = (float32_t) pSrc[n] / 2147483648
 * -------------------------------------------------------------------- */
#include <stdint.h>

void arm_q31_to_float(
  int32_t * pSrc,
  float * pDst,
  uint32_t blockSize)
{
  uint32_t i;
  for (i = 0u; i < blockSize; i++)
  {
    pDst[i] = ((float)pSrc[i]) / 2147483648.0f;
  }
}