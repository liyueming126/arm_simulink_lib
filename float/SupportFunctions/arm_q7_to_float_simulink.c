/* ----------------------------------------------------------------------
 * arm_q7_to_float_simulink.c
 * Simplified self-contained implementation of arm_q7_to_float for Simulink.
 * Converts Q7 (8-bit integer) vector to float.
 * Formula: pDst[n] = (float32_t) pSrc[n] / 128
 * -------------------------------------------------------------------- */
#include <stdint.h>

void arm_q7_to_float(
  int8_t * pSrc,
  float * pDst,
  uint32_t blockSize)
{
  uint32_t i;
  for (i = 0u; i < blockSize; i++)
  {
    pDst[i] = ((float)pSrc[i]) / 128.0f;
  }
}