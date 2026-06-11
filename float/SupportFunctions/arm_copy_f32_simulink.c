/* ----------------------------------------------------------------------
 * arm_copy_f32_simulink.c
 * Simplified self-contained implementation of arm_copy_f32 for Simulink.
 * Copies data from source to destination.
 * -------------------------------------------------------------------- */
#include <stdint.h>

void arm_copy_f32(
  float * pSrc,
  float * pDst,
  uint32_t blockSize)
{
  uint32_t i;
  for (i = 0u; i < blockSize; i++)
  {
    *pDst++ = *pSrc++;
  }
}