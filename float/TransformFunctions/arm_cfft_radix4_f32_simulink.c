/* ----------------------------------------------------------------------
 * arm_cfft_radix4_f32_simulink.c
 * Self-contained Radix-4 CFFT/CIFFT for Simulink.
 * Based on CMSIS arm_cfft_radix4_f32.c (CM0 path).
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Instance structure for Radix-4 CFFT/CIFFT */
typedef struct {
    uint16_t fftLen;
    uint8_t ifftFlag;
    uint8_t bitReverseFlag;
    float *pTwiddle;
    uint16_t *pBitRevTable;
    uint16_t twidCoefModifier;
    uint16_t bitRevFactor;
    float onebyfftLen;
} arm_cfft_radix4_instance_f32;

/* ----------------------------------------------------------------
 * arm_radix4_butterfly_f32 - Forward radix-4 butterfly (CM0 path)
 * ---------------------------------------------------------------- */
static void arm_radix4_butterfly_f32(
  float * pSrc,
  uint16_t fftLen,
  float * pCoef,
  uint16_t twidCoefModifier)
{
  float t1, t2, r1, r2, s1, s2;
  float co1, co2, co3, si1, si2, si3;
  uint32_t ia1, ia2, ia3;
  uint32_t i0, i1, i2, i3;
  uint32_t n1, n2, j, k;

  n2 = fftLen;
  n1 = n2;
  for (k = fftLen; k > 1u; k >>= 2u)
  {
    n1 = n2;
    n2 >>= 2u;
    ia1 = 0u;

    j = 0;
    do
    {
      ia2 = ia1 + ia1;
      ia3 = ia2 + ia1;
      co1 = pCoef[ia1 * 2u];
      si1 = pCoef[(ia1 * 2u) + 1u];
      co2 = pCoef[ia2 * 2u];
      si2 = pCoef[(ia2 * 2u) + 1u];
      co3 = pCoef[ia3 * 2u];
      si3 = pCoef[(ia3 * 2u) + 1u];

      ia1 = ia1 + twidCoefModifier;

      i0 = j;
      do
      {
        i1 = i0 + n2;
        i2 = i1 + n2;
        i3 = i2 + n2;

        r1 = pSrc[(2u * i0)] + pSrc[(2u * i2)];
        r2 = pSrc[(2u * i0)] - pSrc[(2u * i2)];

        s1 = pSrc[(2u * i0) + 1u] + pSrc[(2u * i2) + 1u];
        s2 = pSrc[(2u * i0) + 1u] - pSrc[(2u * i2) + 1u];

        t1 = pSrc[2u * i1] + pSrc[2u * i3];
        pSrc[2u * i0] = r1 + t1;
        r1 = r1 - t1;

        t2 = pSrc[(2u * i1) + 1u] + pSrc[(2u * i3) + 1u];
        pSrc[(2u * i0) + 1u] = s1 + t2;
        s1 = s1 - t2;

        t1 = pSrc[(2u * i1) + 1u] - pSrc[(2u * i3) + 1u];
        t2 = pSrc[2u * i1] - pSrc[2u * i3];

        pSrc[2u * i1] = (r1 * co2) + (s1 * si2);
        pSrc[(2u * i1) + 1u] = (s1 * co2) - (r1 * si2);

        r1 = r2 + t1;
        r2 = r2 - t1;
        s1 = s2 - t2;
        s2 = s2 + t2;

        pSrc[2u * i2] = (r1 * co1) + (s1 * si1);
        pSrc[(2u * i2) + 1u] = (s1 * co1) - (r1 * si1);

        pSrc[2u * i3] = (r2 * co3) + (s2 * si3);
        pSrc[(2u * i3) + 1u] = (s2 * co3) - (r2 * si3);

        i0 += n1;
      } while( i0 < fftLen);
      j++;
    } while(j <= (n2 - 1u));
    twidCoefModifier <<= 2u;
  }
}

/* ----------------------------------------------------------------
 * arm_radix4_butterfly_inverse_f32 - Inverse radix-4 butterfly (CM0 path)
 * ---------------------------------------------------------------- */
static void arm_radix4_butterfly_inverse_f32(
  float * pSrc,
  uint16_t fftLen,
  float * pCoef,
  uint16_t twidCoefModifier,
  float onebyfftLen)
{
  float t1, t2, r1, r2, s1, s2;
  float co1, co2, co3, si1, si2, si3;
  uint32_t ia1, ia2, ia3;
  uint32_t i0, i1, i2, i3;
  uint32_t n1, n2, j, k;

  n2 = fftLen;
  n1 = n2;

  for (k = fftLen; k > 4u; k >>= 2u)
  {
    n1 = n2;
    n2 >>= 2u;
    ia1 = 0u;

    j = 0;
    do
    {
      ia2 = ia1 + ia1;
      ia3 = ia2 + ia1;
      co1 = pCoef[ia1 * 2u];
      si1 = pCoef[(ia1 * 2u) + 1u];
      co2 = pCoef[ia2 * 2u];
      si2 = pCoef[(ia2 * 2u) + 1u];
      co3 = pCoef[ia3 * 2u];
      si3 = pCoef[(ia3 * 2u) + 1u];

      ia1 = ia1 + twidCoefModifier;

      i0 = j;
      do
      {
        i1 = i0 + n2;
        i2 = i1 + n2;
        i3 = i2 + n2;

        r1 = pSrc[(2u * i0)] + pSrc[(2u * i2)];
        r2 = pSrc[(2u * i0)] - pSrc[(2u * i2)];

        s1 = pSrc[(2u * i0) + 1u] + pSrc[(2u * i2) + 1u];
        s2 = pSrc[(2u * i0) + 1u] - pSrc[(2u * i2) + 1u];

        t1 = pSrc[2u * i1] + pSrc[2u * i3];
        pSrc[2u * i0] = r1 + t1;
        r1 = r1 - t1;

        t2 = pSrc[(2u * i1) + 1u] + pSrc[(2u * i3) + 1u];
        pSrc[(2u * i0) + 1u] = s1 + t2;
        s1 = s1 - t2;

        t1 = pSrc[(2u * i1) + 1u] - pSrc[(2u * i3) + 1u];
        t2 = pSrc[2u * i1] - pSrc[2u * i3];

        pSrc[2u * i1] = (r1 * co2) - (s1 * si2);
        pSrc[(2u * i1) + 1u] = (s1 * co2) + (r1 * si2);

        r1 = r2 - t1;
        r2 = r2 + t1;
        s1 = s2 + t2;
        s2 = s2 - t2;

        pSrc[2u * i2] = (r1 * co1) - (s1 * si1);
        pSrc[(2u * i2) + 1u] = (s1 * co1) + (r1 * si1);

        pSrc[2u * i3] = (r2 * co3) - (s2 * si3);
        pSrc[(2u * i3) + 1u] = (s2 * co3) + (r2 * si3);

        i0 += n1;
      } while( i0 < fftLen);
      j++;
    } while(j <= (n2 - 1u));
    twidCoefModifier <<= 2u;
  }

  /* Last stage with scaling by onebyfftLen */
  n1 = n2;
  n2 >>= 2u;

  for (i0 = 0u; i0 <= (fftLen - n1); i0 += n1)
  {
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    r1 = pSrc[2u * i0] + pSrc[2u * i2];
    r2 = pSrc[2u * i0] - pSrc[2u * i2];

    s1 = pSrc[(2u * i0) + 1u] + pSrc[(2u * i2) + 1u];
    s2 = pSrc[(2u * i0) + 1u] - pSrc[(2u * i2) + 1u];

    t1 = pSrc[2u * i1] + pSrc[2u * i3];
    pSrc[2u * i0] = (r1 + t1) * onebyfftLen;
    r1 = r1 - t1;

    t2 = pSrc[(2u * i1) + 1u] + pSrc[(2u * i3) + 1u];
    pSrc[(2u * i0) + 1u] = (s1 + t2) * onebyfftLen;
    s1 = s1 - t2;

    t1 = pSrc[(2u * i1) + 1u] - pSrc[(2u * i3) + 1u];
    t2 = pSrc[2u * i1] - pSrc[2u * i3];

    pSrc[2u * i1] = r1 * onebyfftLen;
    pSrc[(2u * i1) + 1u] = s1 * onebyfftLen;

    r1 = r2 - t1;
    r2 = r2 + t1;
    s1 = s2 + t2;
    s2 = s2 - t2;

    pSrc[2u * i2] = r1 * onebyfftLen;
    pSrc[(2u * i2) + 1u] = s1 * onebyfftLen;

    pSrc[2u * i3] = r2 * onebyfftLen;
    pSrc[(2u * i3) + 1u] = s2 * onebyfftLen;
  }
}

/* ----------------------------------------------------------------
 * arm_bitreversal_f32 for Radix-4
 * ---------------------------------------------------------------- */
static void arm_bitreversal_f32_r4(
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

/* ----------------------------------------------------------------
 * arm_cfft_radix4_f32 - Main entry point
 * ---------------------------------------------------------------- */
void arm_cfft_radix4_f32(
  const arm_cfft_radix4_instance_f32 * S,
  float * pSrc)
{
  if(S->ifftFlag == 1u)
  {
    arm_radix4_butterfly_inverse_f32(pSrc, S->fftLen, S->pTwiddle,
      S->twidCoefModifier, S->onebyfftLen);
  }
  else
  {
    arm_radix4_butterfly_f32(pSrc, S->fftLen, S->pTwiddle,
      S->twidCoefModifier);
  }

  if(S->bitReverseFlag == 1u)
  {
    arm_bitreversal_f32_r4(pSrc, S->fftLen, S->bitRevFactor, S->pBitRevTable);
  }
}