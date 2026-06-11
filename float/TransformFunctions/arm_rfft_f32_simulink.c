/* ----------------------------------------------------------------------
 * arm_rfft_f32_simulink.c
 * Self-contained RFFT/RIFFT for Simulink (deprecated API).
 * Based on CMSIS arm_rfft_f32.c with radix-4 CFFT.
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* RFFT instance */
typedef struct {
    uint32_t fftLenReal;
    uint32_t fftLenBy2;
    uint8_t ifftFlagR;
    uint8_t bitReverseFlagR;
    uint32_t twidCoefRModifier;
    float *pTwiddleAReal;
    float *pTwiddleBReal;
    void *pCfft;
} arm_rfft_instance_f32;

/* Radix-4 CFFT instance */
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
 * arm_radix4_butterfly_f32 (CM0 path)
 * ---------------------------------------------------------------- */
static void arm_radix4_butterfly_f32_r(
  float * pSrc, uint16_t fftLen, float * pCoef, uint16_t twidCoefModifier)
{
  float t1, t2, r1, r2, s1, s2, co1, co2, co3, si1, si2, si3;
  uint32_t ia1, ia2, ia3, i0, i1, i2, i3, n1, n2, j, k;
  n2 = fftLen; n1 = n2;
  for (k = fftLen; k > 1u; k >>= 2u) {
    n1 = n2; n2 >>= 2u; ia1 = 0u; j = 0;
    do {
      ia2 = ia1 + ia1; ia3 = ia2 + ia1;
      co1 = pCoef[ia1 * 2u]; si1 = pCoef[(ia1 * 2u) + 1u];
      co2 = pCoef[ia2 * 2u]; si2 = pCoef[(ia2 * 2u) + 1u];
      co3 = pCoef[ia3 * 2u]; si3 = pCoef[(ia3 * 2u) + 1u];
      ia1 = ia1 + twidCoefModifier; i0 = j;
      do {
        i1 = i0 + n2; i2 = i1 + n2; i3 = i2 + n2;
        r1 = pSrc[2*i0] + pSrc[2*i2]; r2 = pSrc[2*i0] - pSrc[2*i2];
        s1 = pSrc[2*i0+1] + pSrc[2*i2+1]; s2 = pSrc[2*i0+1] - pSrc[2*i2+1];
        t1 = pSrc[2*i1] + pSrc[2*i3];
        pSrc[2*i0] = r1 + t1; r1 = r1 - t1;
        t2 = pSrc[2*i1+1] + pSrc[2*i3+1];
        pSrc[2*i0+1] = s1 + t2; s1 = s1 - t2;
        t1 = pSrc[2*i1+1] - pSrc[2*i3+1];
        t2 = pSrc[2*i1] - pSrc[2*i3];
        pSrc[2*i1] = (r1 * co2) + (s1 * si2);
        pSrc[2*i1+1] = (s1 * co2) - (r1 * si2);
        r1 = r2 + t1; r2 = r2 - t1; s1 = s2 - t2; s2 = s2 + t2;
        pSrc[2*i2] = (r1 * co1) + (s1 * si1);
        pSrc[2*i2+1] = (s1 * co1) - (r1 * si1);
        pSrc[2*i3] = (r2 * co3) + (s2 * si3);
        pSrc[2*i3+1] = (s2 * co3) - (r2 * si3);
        i0 += n1;
      } while(i0 < fftLen); j++;
    } while(j <= (n2 - 1u));
    twidCoefModifier <<= 2u;
  }
}

/* ----------------------------------------------------------------
 * arm_radix4_butterfly_inverse_f32 (CM0 path)
 * ---------------------------------------------------------------- */
static void arm_radix4_butterfly_inverse_f32_r(
  float * pSrc, uint16_t fftLen, float * pCoef,
  uint16_t twidCoefModifier, float onebyfftLen)
{
  float t1, t2, r1, r2, s1, s2, co1, co2, co3, si1, si2, si3;
  uint32_t ia1, ia2, ia3, i0, i1, i2, i3, n1, n2, j, k;
  n2 = fftLen; n1 = n2;
  for (k = fftLen; k > 4u; k >>= 2u) {
    n1 = n2; n2 >>= 2u; ia1 = 0u; j = 0;
    do {
      ia2 = ia1 + ia1; ia3 = ia2 + ia1;
      co1 = pCoef[ia1*2u]; si1 = pCoef[ia1*2u+1];
      co2 = pCoef[ia2*2u]; si2 = pCoef[ia2*2u+1];
      co3 = pCoef[ia3*2u]; si3 = pCoef[ia3*2u+1];
      ia1 = ia1 + twidCoefModifier; i0 = j;
      do {
        i1 = i0+n2; i2 = i1+n2; i3 = i2+n2;
        r1 = pSrc[2*i0] + pSrc[2*i2]; r2 = pSrc[2*i0] - pSrc[2*i2];
        s1 = pSrc[2*i0+1] + pSrc[2*i2+1]; s2 = pSrc[2*i0+1] - pSrc[2*i2+1];
        t1 = pSrc[2*i1] + pSrc[2*i3];
        pSrc[2*i0] = r1 + t1; r1 = r1 - t1;
        t2 = pSrc[2*i1+1] + pSrc[2*i3+1];
        pSrc[2*i0+1] = s1 + t2; s1 = s1 - t2;
        t1 = pSrc[2*i1+1] - pSrc[2*i3+1];
        t2 = pSrc[2*i1] - pSrc[2*i3];
        pSrc[2*i1] = (r1 * co2) - (s1 * si2);
        pSrc[2*i1+1] = (s1 * co2) + (r1 * si2);
        r1 = r2 - t1; r2 = r2 + t1; s1 = s2 + t2; s2 = s2 - t2;
        pSrc[2*i2] = (r1 * co1) - (s1 * si1);
        pSrc[2*i2+1] = (s1 * co1) + (r1 * si1);
        pSrc[2*i3] = (r2 * co3) - (s2 * si3);
        pSrc[2*i3+1] = (s2 * co3) + (r2 * si3);
        i0 += n1;
      } while(i0 < fftLen); j++;
    } while(j <= (n2 - 1u)); twidCoefModifier <<= 2u;
  }
  n1 = n2; n2 >>= 2u;
  for (i0 = 0u; i0 <= (fftLen - n1); i0 += n1) {
    i1 = i0+n2; i2 = i1+n2; i3 = i2+n2;
    r1 = pSrc[2*i0] + pSrc[2*i2]; r2 = pSrc[2*i0] - pSrc[2*i2];
    s1 = pSrc[2*i0+1] + pSrc[2*i2+1]; s2 = pSrc[2*i0+1] - pSrc[2*i2+1];
    t1 = pSrc[2*i1] + pSrc[2*i3];
    pSrc[2*i0] = (r1 + t1) * onebyfftLen; r1 = r1 - t1;
    t2 = pSrc[2*i1+1] + pSrc[2*i3+1];
    pSrc[2*i0+1] = (s1 + t2) * onebyfftLen; s1 = s1 - t2;
    t1 = pSrc[2*i1+1] - pSrc[2*i3+1];
    t2 = pSrc[2*i1] - pSrc[2*i3];
    pSrc[2*i1] = r1 * onebyfftLen; pSrc[2*i1+1] = s1 * onebyfftLen;
    r1 = r2 - t1; r2 = r2 + t1; s1 = s2 + t2; s2 = s2 - t2;
    pSrc[2*i2] = r1 * onebyfftLen; pSrc[2*i2+1] = s1 * onebyfftLen;
    pSrc[2*i3] = r2 * onebyfftLen; pSrc[2*i3+1] = s2 * onebyfftLen;
  }
}

/* ----------------------------------------------------------------
 * arm_bitreversal_f32
 * ---------------------------------------------------------------- */
static void arm_bitreversal_f32_rf(
  float * pSrc, uint16_t fftSize, uint16_t bitRevFactor, uint16_t * pBitRevTab)
{
  uint16_t fftLenBy2, fftLenBy2p1, i, j; float in;
  j = 0u; fftLenBy2 = fftSize >> 1u; fftLenBy2p1 = (fftSize>>1u)+1u;
  for (i = 0u; i <= (fftLenBy2 - 2u); i += 2u) {
    if(i < j) {
      in = pSrc[2u*i]; pSrc[2u*i] = pSrc[2u*j]; pSrc[2u*j] = in;
      in = pSrc[2u*i+1]; pSrc[2u*i+1] = pSrc[2u*j+1]; pSrc[2u*j+1] = in;
      in = pSrc[2u*(i+fftLenBy2p1)]; pSrc[2u*(i+fftLenBy2p1)] = pSrc[2u*(j+fftLenBy2p1)]; pSrc[2u*(j+fftLenBy2p1)] = in;
      in = pSrc[2u*(i+fftLenBy2p1)+1]; pSrc[2u*(i+fftLenBy2p1)+1] = pSrc[2u*(j+fftLenBy2p1)+1]; pSrc[2u*(j+fftLenBy2p1)+1] = in;
    }
    in = pSrc[2u*(i+1)]; pSrc[2u*(i+1)] = pSrc[2u*(j+fftLenBy2)]; pSrc[2u*(j+fftLenBy2)] = in;
    in = pSrc[2u*(i+1)+1]; pSrc[2u*(i+1)+1] = pSrc[2u*(j+fftLenBy2)+1]; pSrc[2u*(j+fftLenBy2)+1] = in;
    j = *pBitRevTab; pBitRevTab += bitRevFactor;
  }
}

/* ----------------------------------------------------------------
 * arm_split_rfft_f32 - Core RFFT processing
 * ---------------------------------------------------------------- */
static void arm_split_rfft_f32(
  float * pSrc, uint32_t fftLen, float * pATable, float * pBTable,
  float * pDst, uint32_t modifier)
{
  uint32_t i;
  float outR, outI;
  float *pCoefA, *pCoefB;
  float CoefA1, CoefA2, CoefB1;
  float *pDst1 = &pDst[2], *pDst2 = &pDst[(4u * fftLen) - 1u];
  float *pSrc1 = &pSrc[2], *pSrc2 = &pSrc[(2u * fftLen) - 1u];

  pCoefA = &pATable[modifier * 2u];
  pCoefB = &pBTable[modifier * 2u];

  i = fftLen - 1u;
  while(i > 0u) {
    CoefA1 = *pCoefA++; CoefA2 = *pCoefA;
    outR = *pSrc1 * CoefA1;
    outI = *pSrc1++ * CoefA2;
    outR -= (*pSrc1 + *pSrc2) * CoefA2;
    outI += *pSrc1++ * CoefA1;
    CoefB1 = *pCoefB;
    outI -= *pSrc2-- * CoefB1;
    outI -= *pSrc2 * CoefA2;
    outR += *pSrc2-- * CoefB1;
    *pDst1++ = outR; *pDst1++ = outI;
    *pDst2-- = -outI; *pDst2-- = outR;
    pCoefB = pCoefB + (modifier * 2u);
    pCoefA = pCoefA + ((modifier * 2u) - 1u);
    i--;
  }
  pDst[2u * fftLen] = pSrc[0] - pSrc[1];
  pDst[(2u * fftLen) + 1u] = 0.0f;
  pDst[0] = pSrc[0] + pSrc[1];
  pDst[1] = 0.0f;
}

/* ----------------------------------------------------------------
 * arm_split_rifft_f32 - Core RIFFT processing
 * ---------------------------------------------------------------- */
static void arm_split_rifft_f32(
  float * pSrc, uint32_t fftLen, float * pATable, float * pBTable,
  float * pDst, uint32_t modifier)
{
  float outR, outI;
  float *pCoefA, *pCoefB;
  float CoefA1, CoefA2, CoefB1;
  float *pSrc1 = &pSrc[0], *pSrc2 = &pSrc[(2u * fftLen) + 1u];

  pCoefA = &pATable[0];
  pCoefB = &pBTable[0];

  while(fftLen > 0u) {
    CoefA1 = *pCoefA++; CoefA2 = *pCoefA;
    outR = *pSrc1 * CoefA1;
    outI = -(*pSrc1++) * CoefA2;
    outR += (*pSrc1 + *pSrc2) * CoefA2;
    outI += (*pSrc1++) * CoefA1;
    CoefB1 = *pCoefB;
    outI -= *pSrc2-- * CoefB1;
    outR += *pSrc2 * CoefB1;
    outI += *pSrc2-- * CoefA2;
    *pDst++ = outR; *pDst++ = outI;
    pCoefB = pCoefB + (modifier * 2u);
    pCoefA = pCoefA + ((modifier * 2u) - 1u);
    fftLen--;
  }
}

/* ----------------------------------------------------------------
 * arm_rfft_f32 - Main entry point
 * ---------------------------------------------------------------- */
void arm_rfft_f32(
  const arm_rfft_instance_f32 * S,
  float * pSrc,
  float * pDst)
{
  /* For the RFFT, we need the CFFT instance */
  arm_cfft_radix4_instance_f32 *S_CFFT = (arm_cfft_radix4_instance_f32 *)S->pCfft;

  if(S->ifftFlagR == 1u) {
    arm_split_rifft_f32(pSrc, S->fftLenBy2, S->pTwiddleAReal,
                        S->pTwiddleBReal, pDst, S->twidCoefRModifier);
    arm_radix4_butterfly_inverse_f32_r(pDst, S_CFFT->fftLen,
                                     S_CFFT->pTwiddle,
                                     S_CFFT->twidCoefModifier,
                                     S_CFFT->onebyfftLen);
    if(S->bitReverseFlagR == 1u) {
      arm_bitreversal_f32_rf(pDst, S_CFFT->fftLen,
                           S_CFFT->bitRevFactor, S_CFFT->pBitRevTable);
    }
  } else {
    arm_radix4_butterfly_f32_r(pSrc, S_CFFT->fftLen,
                              S_CFFT->pTwiddle, S_CFFT->twidCoefModifier);
    if(S->bitReverseFlagR == 1u) {
      arm_bitreversal_f32_rf(pSrc, S_CFFT->fftLen,
                           S_CFFT->bitRevFactor, S_CFFT->pBitRevTable);
    }
    arm_split_rfft_f32(pSrc, S->fftLenBy2, S->pTwiddleAReal,
                       S->pTwiddleBReal, pDst, S->twidCoefRModifier);
  }
}