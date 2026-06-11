/* ----------------------------------------------------------------------
 * arm_cfft_radix2_f32_simulink.c
 * Simplified self-contained Radix-2 CFFT/CIFFT for Simulink.
 * Based on CMSIS arm_cfft_radix2_f32.c (CM0 path).
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Instance structure for Radix-2 CFFT/CIFFT */
typedef struct {
    uint16_t fftLen;
    uint8_t ifftFlag;
    uint8_t bitReverseFlag;
    float *pTwiddle;
    uint16_t *pBitRevTable;
    uint16_t twidCoefModifier;
    uint16_t bitRevFactor;
    float onebyfftLen;
} arm_cfft_radix2_instance_f32;

/* ----------------------------------------------------------------
 * arm_radix2_butterfly_f32 - Forward radix-2 butterfly
 * ---------------------------------------------------------------- */
static void arm_radix2_butterfly_f32(
  float * pSrc,
  uint32_t fftLen,
  float * pCoef,
  uint16_t twidCoefModifier)
{
  uint32_t i, j, k, l;
  uint32_t n1, n2, ia;
  float xt, yt, cosVal, sinVal;
  float p0, p1, p2, p3;
  float a0, a1;

  n2 = fftLen;

  /* Loop over stages */
  for (k = fftLen; k > 1; k = k >> 1)
  {
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    /* Loop over groups */
    j = 0;
    do
    {
      cosVal = pCoef[ia * 2];
      sinVal = pCoef[(ia * 2) + 1];
      ia += twidCoefModifier;

      /* Loop over butterflies */
      i = j;
      do
      {
        l = i + n2;
        a0 = pSrc[2 * i] + pSrc[2 * l];
        xt = pSrc[2 * i] - pSrc[2 * l];

        yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
        a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];

        p0 = xt * cosVal;
        p1 = yt * sinVal;
        p2 = yt * cosVal;
        p3 = xt * sinVal;

        pSrc[2 * i]     = a0;
        pSrc[2 * i + 1] = a1;

        pSrc[2 * l]     = p0 + p1;
        pSrc[2 * l + 1] = p2 - p3;

        i += n1;
      } while(i < fftLen);
      j++;
    } while(j < n2);
    twidCoefModifier <<= 1u;
  }
}

/* ----------------------------------------------------------------
 * arm_radix2_butterfly_inverse_f32 - Inverse radix-2 butterfly
 * ---------------------------------------------------------------- */
static void arm_radix2_butterfly_inverse_f32(
  float * pSrc,
  uint32_t fftLen,
  float * pCoef,
  uint16_t twidCoefModifier,
  float onebyfftLen)
{
  uint32_t i, j, k, l;
  uint32_t n1, n2, ia;
  float xt, yt, cosVal, sinVal;
  float p0, p1, p2, p3;
  float a0, a1;

  n2 = fftLen;

  /* Loop over stages */
  for (k = fftLen; k > 2; k = k >> 1)
  {
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    /* Loop over groups */
    j = 0;
    do
    {
      cosVal = pCoef[ia * 2];
      sinVal = pCoef[(ia * 2) + 1];
      ia = ia + twidCoefModifier;

      /* Loop over butterflies */
      i = j;
      do
      {
        l = i + n2;
        a0 = pSrc[2 * i] + pSrc[2 * l];
        xt = pSrc[2 * i] - pSrc[2 * l];

        yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
        a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];

        p0 = xt * cosVal;
        p1 = yt * sinVal;
        p2 = yt * cosVal;
        p3 = xt * sinVal;

        pSrc[2 * i]     = a0;
        pSrc[2 * i + 1] = a1;

        pSrc[2 * l]     = p0 - p1;
        pSrc[2 * l + 1] = p2 + p3;

        i += n1;
      } while( i < fftLen );
      j++;
    } while( j < n2 );

    twidCoefModifier = twidCoefModifier << 1u;
  }

  /* Last stage with scaling */
  n1 = n2;
  n2 = n2 >> 1;

  for (i = 0; i < fftLen; i += n1)
  {
    l = i + n2;

    a0 = pSrc[2 * i] + pSrc[2 * l];
    xt = pSrc[2 * i] - pSrc[2 * l];

    a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];
    yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];

    p0 = a0 * onebyfftLen;
    p2 = xt * onebyfftLen;
    p1 = a1 * onebyfftLen;
    p3 = yt * onebyfftLen;

    pSrc[2 * i]     = p0;
    pSrc[2u * l]    = p2;
    pSrc[2 * i + 1] = p1;
    pSrc[2u * l + 1u] = p3;
  }
}

/* ----------------------------------------------------------------
 * arm_bitreversal_f32 compatible version
 * ---------------------------------------------------------------- */
static void arm_bitreversal_f32_simple(
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

/* Generate twiddle factors: exp(-j*2*pi*k/N) for k=0..N/2-1 */
static float *generate_twiddle_radix2_fn(uint16_t n)
{
  uint32_t i;
  uint32_t len = n / 2u;
  float *table = (float *)malloc(len * 2u * sizeof(float));
  if (!table) return 0;
  for (i = 0; i < len; i++) {
    double angle = -2.0 * 3.14159265358979323846 * (double)i / (double)n;
    table[2u * i]     = (float)cos(angle);
    table[2u * i + 1u] = (float)sin(angle);
  }
  return table;
}

/* Generate bit reversal table */
static uint16_t *generate_bitrev_radix2_fn(uint16_t n)
{
  uint32_t i;
  uint16_t *table;
  uint32_t bits = 0;
  uint32_t temp = n;
  while (temp > 1) { temp >>= 1; bits++; }
  table = (uint16_t *)malloc((n - 4) * sizeof(uint16_t));
  if (!table) return 0;
  for (i = 0; i < n - 4; i++) {
    uint32_t rev = 0, idx = i + 1, b;
    for (b = 0; b < bits; b++) {
      rev = (rev << 1) | (idx & 1);
      idx >>= 1;
    }
    table[i] = (uint16_t)(rev >> 1);
  }
  return table;
}

/* ----------------------------------------------------------------
 * arm_cfft_radix2_f32 - Main entry point
 * ---------------------------------------------------------------- */
void arm_cfft_radix2_f32(
  const arm_cfft_radix2_instance_f32 * S,
  float * pSrc)
{
  if(S->ifftFlag == 1u)
  {
    arm_radix2_butterfly_inverse_f32(pSrc, S->fftLen, S->pTwiddle,
      S->twidCoefModifier, S->onebyfftLen);
  }
  else
  {
    arm_radix2_butterfly_f32(pSrc, S->fftLen, S->pTwiddle,
      S->twidCoefModifier);
  }

  if(S->bitReverseFlag == 1u)
  {
    arm_bitreversal_f32_simple(pSrc, S->fftLen, S->bitRevFactor, S->pBitRevTable);
  }
}