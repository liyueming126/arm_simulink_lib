/* ----------------------------------------------------------------------
 * arm_float_to_q31_simulink.c
 * Simplified self-contained implementation of arm_float_to_q31 for Simulink.
 * Converts float vector to Q31 (32-bit integer).
 * Formula: pDst[n] = (q31_t)(pSrc[n] * 2147483648), with saturation.
 * -------------------------------------------------------------------- */
#include <stdint.h>

static int32_t sat_q31(int64_t val)
{
    if (val > 2147483647LL) return 2147483647;
    if (val < -2147483648LL) return -2147483648;
    return (int32_t)val;
}

void arm_float_to_q31(
  float * pSrc,
  int32_t * pDst,
  uint32_t blockSize)
{
  uint32_t i;
  for (i = 0u; i < blockSize; i++)
  {
    float v = pSrc[i] * 2147483648.0f;
    if (v > 2147483647.0f)
      pDst[i] = 2147483647;
    else if (v < -2147483648.0f)
      pDst[i] = -2147483648;
    else
    {
      int64_t iv = (int64_t)v;
      if (v < 0.0f && (float)((double)iv) != v) iv--;
      pDst[i] = (int32_t)iv;
    }
  }
}