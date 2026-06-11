/* ----------------------------------------------------------------------
 * arm_fill_f32_simulink.c
 * Simplified self-contained implementation of arm_fill_f32 for Simulink.
 * Fills a buffer with a constant value.
 * -------------------------------------------------------------------- */
#include <stdint.h>

void arm_fill_f32(
  float value,
  float * pDst,
  uint32_t blockSize)
{
  uint32_t i;
  for (i = 0u; i < blockSize; i++)
  {
    *pDst++ = value;
  }
}