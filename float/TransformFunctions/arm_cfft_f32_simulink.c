/* ----------------------------------------------------------------------
 * arm_cfft_f32_simulink.c
 * Self-contained mixed-radix CFFT for Simulink.
 * Based on CMSIS arm_cfft_f32.c, arm_cfft_radix8_f32.c, and arm_bitreversal.c.
 * Supports FFT lengths: 16, 32, 64, 128, 256, 512, 1024, 2048, 4096.
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* --------------------------------------------------------------------
 * Instance structure for the floating-point CFFT
 * -------------------------------------------------------------------- */
typedef struct {
    uint16_t fftLen;
    const float *pTwiddle;
    const uint16_t *pBitRevTable;
    uint16_t bitRevLength;
} arm_cfft_instance_f32;

/* ----------------------------------------------------------------
 * Utility: bit-reverse an index
 * ---------------------------------------------------------------- */
static uint32_t bitrev_index(uint32_t idx, uint32_t bits)
{
    uint32_t rev = 0, i;
    for (i = 0; i < bits; i++) {
        rev = (rev << 1) | (idx & 1);
        idx >>= 1;
    }
    return rev;
}

/* ----------------------------------------------------------------
 * arm_radix8_butterfly_f32 - Radix-8 butterfly core
 * ---------------------------------------------------------------- */
static void arm_radix8_butterfly_f32(
  float * pSrc,
  uint16_t fftLen,
  const float * pCoef,
  uint16_t twidCoefModifier)
{
  uint32_t ia1, ia2, ia3, ia4, ia5, ia6, ia7;
  uint32_t i1, i2, i3, i4, i5, i6, i7, i8;
  uint32_t id;
  uint32_t n1, n2, j;

  float r1, r2, r3, r4, r5, r6, r7, r8;
  float t1, t2;
  float s1, s2, s3, s4, s5, s6, s7, s8;
  float p1, p2, p3, p4;
  float co2, co3, co4, co5, co6, co7, co8;
  float si2, si3, si4, si5, si6, si7, si8;
  const float C81 = 0.70710678118f;

  n2 = fftLen;

  do
  {
    n1 = n2;
    n2 = n2 >> 3;
    i1 = 0;

    do
    {
      i2 = i1 + n2;
      i3 = i2 + n2;
      i4 = i3 + n2;
      i5 = i4 + n2;
      i6 = i5 + n2;
      i7 = i6 + n2;
      i8 = i7 + n2;
      r1 = pSrc[2 * i1] + pSrc[2 * i5];
      r5 = pSrc[2 * i1] - pSrc[2 * i5];
      r2 = pSrc[2 * i2] + pSrc[2 * i6];
      r6 = pSrc[2 * i2] - pSrc[2 * i6];
      r3 = pSrc[2 * i3] + pSrc[2 * i7];
      r7 = pSrc[2 * i3] - pSrc[2 * i7];
      r4 = pSrc[2 * i4] + pSrc[2 * i8];
      r8 = pSrc[2 * i4] - pSrc[2 * i8];
      t1 = r1 - r3; r1 = r1 + r3; r3 = r2 - r4; r2 = r2 + r4;
      pSrc[2 * i1] = r1 + r2;
      pSrc[2 * i5] = r1 - r2;
      r1 = pSrc[2 * i1 + 1] + pSrc[2 * i5 + 1];
      s5 = pSrc[2 * i1 + 1] - pSrc[2 * i5 + 1];
      r2 = pSrc[2 * i2 + 1] + pSrc[2 * i6 + 1];
      s6 = pSrc[2 * i2 + 1] - pSrc[2 * i6 + 1];
      s3 = pSrc[2 * i3 + 1] + pSrc[2 * i7 + 1];
      s7 = pSrc[2 * i3 + 1] - pSrc[2 * i7 + 1];
      r4 = pSrc[2 * i4 + 1] + pSrc[2 * i8 + 1];
      s8 = pSrc[2 * i4 + 1] - pSrc[2 * i8 + 1];
      t2 = r1 - s3; r1 = r1 + s3; s3 = r2 - r4; r2 = r2 + r4;
      pSrc[2 * i1 + 1] = r1 + r2;
      pSrc[2 * i5 + 1] = r1 - r2;
      pSrc[2 * i3]     = t1 + s3;
      pSrc[2 * i7]     = t1 - s3;
      pSrc[2 * i3 + 1] = t2 - r3;
      pSrc[2 * i7 + 1] = t2 + r3;
      r1 = (r6 - r8) * C81; r6 = (r6 + r8) * C81;
      r2 = (s6 - s8) * C81; s6 = (s6 + s8) * C81;
      t1 = r5 - r1; r5 = r5 + r1; r8 = r7 - r6; r7 = r7 + r6;
      t2 = s5 - r2; s5 = s5 + r2; s8 = s7 - s6; s7 = s7 + s6;
      pSrc[2 * i2]     = r5 + s7;     pSrc[2 * i8]     = r5 - s7;
      pSrc[2 * i6]     = t1 + s8;     pSrc[2 * i4]     = t1 - s8;
      pSrc[2 * i2 + 1] = s5 - r7;     pSrc[2 * i8 + 1] = s5 + r7;
      pSrc[2 * i6 + 1] = t2 - r8;     pSrc[2 * i4 + 1] = t2 + r8;
      i1 += n1;
    } while(i1 < fftLen);

    if(n2 < 8) break;

    ia1 = 0;
    j = 1;
    do
    {
      id  = ia1 + twidCoefModifier;
      ia1 = id;
      ia2 = ia1 + id; ia3 = ia2 + id; ia4 = ia3 + id;
      ia5 = ia4 + id; ia6 = ia5 + id; ia7 = ia6 + id;

      co2 = pCoef[2 * ia1]; co3 = pCoef[2 * ia2]; co4 = pCoef[2 * ia3];
      co5 = pCoef[2 * ia4]; co6 = pCoef[2 * ia5]; co7 = pCoef[2 * ia6];
      co8 = pCoef[2 * ia7];
      si2 = pCoef[2 * ia1 + 1]; si3 = pCoef[2 * ia2 + 1];
      si4 = pCoef[2 * ia3 + 1]; si5 = pCoef[2 * ia4 + 1];
      si6 = pCoef[2 * ia5 + 1]; si7 = pCoef[2 * ia6 + 1];
      si8 = pCoef[2 * ia7 + 1];

      i1 = j;
      do
      {
        i2 = i1 + n2; i3 = i2 + n2; i4 = i3 + n2;
        i5 = i4 + n2; i6 = i5 + n2; i7 = i6 + n2; i8 = i7 + n2;
        r1 = pSrc[2 * i1] + pSrc[2 * i5];
        r5 = pSrc[2 * i1] - pSrc[2 * i5];
        r2 = pSrc[2 * i2] + pSrc[2 * i6];
        r6 = pSrc[2 * i2] - pSrc[2 * i6];
        r3 = pSrc[2 * i3] + pSrc[2 * i7];
        r7 = pSrc[2 * i3] - pSrc[2 * i7];
        r4 = pSrc[2 * i4] + pSrc[2 * i8];
        r8 = pSrc[2 * i4] - pSrc[2 * i8];
        t1 = r1 - r3; r1 = r1 + r3; r3 = r2 - r4; r2 = r2 + r4;
        pSrc[2 * i1] = r1 + r2;
        r2 = r1 - r2;
        s1 = pSrc[2 * i1 + 1] + pSrc[2 * i5 + 1];
        s5 = pSrc[2 * i1 + 1] - pSrc[2 * i5 + 1];
        s2 = pSrc[2 * i2 + 1] + pSrc[2 * i6 + 1];
        s6 = pSrc[2 * i2 + 1] - pSrc[2 * i6 + 1];
        s3 = pSrc[2 * i3 + 1] + pSrc[2 * i7 + 1];
        s7 = pSrc[2 * i3 + 1] - pSrc[2 * i7 + 1];
        s4 = pSrc[2 * i4 + 1] + pSrc[2 * i8 + 1];
        s8 = pSrc[2 * i4 + 1] - pSrc[2 * i8 + 1];
        t2 = s1 - s3; s1 = s1 + s3; s3 = s2 - s4; s2 = s2 + s4;
        r1 = t1 + s3; t1 = t1 - s3;
        pSrc[2 * i1 + 1] = s1 + s2;
        s2 = s1 - s2; s1 = t2 - r3; t2 = t2 + r3;
        p1 = co5 * r2;  p2 = si5 * s2;  p3 = co5 * s2;  p4 = si5 * r2;
        pSrc[2 * i5]     = p1 + p2;      pSrc[2 * i5 + 1] = p3 - p4;
        p1 = co3 * r1;  p2 = si3 * s1;  p3 = co3 * s1;  p4 = si3 * r1;
        pSrc[2 * i3]     = p1 + p2;      pSrc[2 * i3 + 1] = p3 - p4;
        p1 = co7 * t1;  p2 = si7 * t2;  p3 = co7 * t2;  p4 = si7 * t1;
        pSrc[2 * i7]     = p1 + p2;      pSrc[2 * i7 + 1] = p3 - p4;
        r1 = (r6 - r8) * C81; r6 = (r6 + r8) * C81;
        s1 = (s6 - s8) * C81; s6 = (s6 + s8) * C81;
        t1 = r5 - r1; r5 = r5 + r1; r8 = r7 - r6; r7 = r7 + r6;
        t2 = s5 - s1; s5 = s5 + s1; s8 = s7 - s6; s7 = s7 + s6;
        r1 = r5 + s7; r5 = r5 - s7; r6 = t1 + s8; t1 = t1 - s8;
        s1 = s5 - r7; s5 = s5 + r7; s6 = t2 - r8; t2 = t2 + r8;
        p1 = co2 * r1;  p2 = si2 * s1;  p3 = co2 * s1;  p4 = si2 * r1;
        pSrc[2 * i2]     = p1 + p2;      pSrc[2 * i2 + 1] = p3 - p4;
        p1 = co8 * r5;  p2 = si8 * s5;  p3 = co8 * s5;  p4 = si8 * r5;
        pSrc[2 * i8]     = p1 + p2;      pSrc[2 * i8 + 1] = p3 - p4;
        p1 = co6 * r6;  p2 = si6 * s6;  p3 = co6 * s6;  p4 = si6 * r6;
        pSrc[2 * i6]     = p1 + p2;      pSrc[2 * i6 + 1] = p3 - p4;
        p1 = co4 * t1;  p2 = si4 * t2;  p3 = co4 * t2;  p4 = si4 * t1;
        pSrc[2 * i4]     = p1 + p2;      pSrc[2 * i4 + 1] = p3 - p4;
        i1 += n1;
      } while(i1 < fftLen);
      j++;
    } while(j < n2);
    twidCoefModifier <<= 3;
  } while(n2 > 7);
}

/* ----------------------------------------------------------------
 * arm_cfft_radix8by2_f32 - Two-column DIF stage
 * ---------------------------------------------------------------- */
static void arm_cfft_radix8by2_f32(arm_cfft_instance_f32 * S, float * p1)
{
  uint32_t L = S->fftLen;
  float *pCol1, *pCol2, *pMid1, *pMid2;
  float *p2 = p1 + L;
  const float *tw = (float *)S->pTwiddle;
  float t1[4], t2[4], t3[4], t4[4], twR, twI;
  float m0, m1, m2, m3;
  uint32_t l;

  pCol1 = p1;
  pCol2 = p2;

  L >>= 1;
  pMid1 = p1 + L;
  pMid2 = p2 + L;

  for (l = L >> 2; l > 0; l--)
  {
    t1[0] = p1[0]; t1[1] = p1[1]; t1[2] = p1[2]; t1[3] = p1[3];
    t2[0] = p2[0]; t2[1] = p2[1]; t2[2] = p2[2]; t2[3] = p2[3];
    t3[0] = pMid1[0]; t3[1] = pMid1[1]; t3[2] = pMid1[2]; t3[3] = pMid1[3];
    t4[0] = pMid2[0]; t4[1] = pMid2[1]; t4[2] = pMid2[2]; t4[3] = pMid2[3];

    *p1++ = t1[0] + t2[0]; *p1++ = t1[1] + t2[1];
    *p1++ = t1[2] + t2[2]; *p1++ = t1[3] + t2[3];

    t2[0] = t1[0] - t2[0]; t2[1] = t1[1] - t2[1];
    t2[2] = t1[2] - t2[2]; t2[3] = t1[3] - t2[3];

    *pMid1++ = t3[0] + t4[0]; *pMid1++ = t3[1] + t4[1];
    *pMid1++ = t3[2] + t4[2]; *pMid1++ = t3[3] + t4[3];

    t4[0] = t4[0] - t3[0]; t4[1] = t4[1] - t3[1];
    t4[2] = t4[2] - t3[2]; t4[3] = t4[3] - t3[3];

    twR = *tw++; twI = *tw++;

    m0 = t2[0] * twR; m1 = t2[1] * twI; m2 = t2[1] * twR; m3 = t2[0] * twI;
    *p2++ = m0 + m1; *p2++ = m2 - m3;

    m0 = t4[0] * twI; m1 = t4[1] * twR; m2 = t4[1] * twI; m3 = t4[0] * twR;
    *pMid2++ = m0 - m1; *pMid2++ = m2 + m3;

    twR = *tw++; twI = *tw++;

    m0 = t2[2] * twR; m1 = t2[3] * twI; m2 = t2[3] * twR; m3 = t2[2] * twI;
    *p2++ = m0 + m1; *p2++ = m2 - m3;

    m0 = t4[2] * twI; m1 = t4[3] * twR; m2 = t4[3] * twI; m3 = t4[2] * twR;
    *pMid2++ = m0 - m1; *pMid2++ = m2 + m3;
  }

  arm_radix8_butterfly_f32(pCol1, L, S->pTwiddle, 2u);
  arm_radix8_butterfly_f32(pCol2, L, S->pTwiddle, 2u);
}

/* ----------------------------------------------------------------
 * arm_cfft_radix8by4_f32 - Four-column DIF stage
 * ---------------------------------------------------------------- */
static void arm_cfft_radix8by4_f32(arm_cfft_instance_f32 * S, float * p1)
{
  uint32_t L = S->fftLen >> 1;
  float *pCol1, *pCol2, *pCol3, *pCol4, *pEnd1, *pEnd2, *pEnd3, *pEnd4;
  const float *tw2, *tw3, *tw4;
  float *p2 = p1 + L;
  float *p3 = p2 + L;
  float *p4 = p3 + L;
  float t2[4], t3[4], t4[4], twR, twI;
  float p1ap3_0, p1sp3_0, p1ap3_1, p1sp3_1;
  float m0, m1, m2, m3;
  uint32_t l, twMod2, twMod3, twMod4;

  pCol1 = p1; pCol2 = p2; pCol3 = p3; pCol4 = p4;
  pEnd1 = p2 - 1; pEnd2 = p3 - 1; pEnd3 = p4 - 1; pEnd4 = pEnd3 + L;

  tw2 = tw3 = tw4 = (float *)S->pTwiddle;
  L >>= 1;
  twMod2 = 2; twMod3 = 4; twMod4 = 6;

  p1ap3_0 = p1[0] + p3[0]; p1sp3_0 = p1[0] - p3[0];
  p1ap3_1 = p1[1] + p3[1]; p1sp3_1 = p1[1] - p3[1];

  t2[0] = p1sp3_0 + p2[1] - p4[1];
  t2[1] = p1sp3_1 - p2[0] + p4[0];
  t3[0] = p1ap3_0 - p2[0] - p4[0];
  t3[1] = p1ap3_1 - p2[1] - p4[1];
  t4[0] = p1sp3_0 - p2[1] + p4[1];
  t4[1] = p1sp3_1 + p2[0] - p4[0];
  *p1++ = p1ap3_0 + p2[0] + p4[0];
  *p1++ = p1ap3_1 + p2[1] + p4[1];

  *p2++ = t2[0]; *p2++ = t2[1];
  *p3++ = t3[0]; *p3++ = t3[1];
  *p4++ = t4[0]; *p4++ = t4[1];

  tw2 += twMod2; tw3 += twMod3; tw4 += twMod4;

  for (l = (L - 2) >> 1; l > 0; l--)
  {
    p1ap3_0 = p1[0] + p3[0]; p1sp3_0 = p1[0] - p3[0];
    p1ap3_1 = p1[1] + p3[1]; p1sp3_1 = p1[1] - p3[1];
    t2[0] = p1sp3_0 + p2[1] - p4[1];
    t2[1] = p1sp3_1 - p2[0] + p4[0];
    t3[0] = p1ap3_0 - p2[0] - p4[0];
    t3[1] = p1ap3_1 - p2[1] - p4[1];
    t4[0] = p1sp3_0 - p2[1] + p4[1];
    t4[1] = p1sp3_1 + p2[0] - p4[0];
    *p1++ = p1ap3_0 + p2[0] + p4[0];
    *p1++ = p1ap3_1 + p2[1] + p4[1];

    p1ap3_1 = pEnd1[-1] + pEnd3[-1];
    p1sp3_1 = pEnd1[-1] - pEnd3[-1];
    p1ap3_0 = pEnd1[0] + pEnd3[0];
    p1sp3_0 = pEnd1[0] - pEnd3[0];
    t2[2] = pEnd2[0] - pEnd4[0] + p1sp3_1;
    t2[3] = pEnd1[0] - pEnd3[0] - pEnd2[-1] + pEnd4[-1];
    t3[2] = p1ap3_1 - pEnd2[-1] - pEnd4[-1];
    t3[3] = p1ap3_0 - pEnd2[0] - pEnd4[0];
    t4[2] = pEnd2[0] - pEnd4[0] - p1sp3_1;
    t4[3] = pEnd4[-1] - pEnd2[-1] - p1sp3_0;
    *pEnd1-- = p1ap3_0 + pEnd2[0] + pEnd4[0];
    *pEnd1-- = p1ap3_1 + pEnd2[-1] + pEnd4[-1];

    twR = *tw2++; twI = *tw2++;
    m0 = t2[0] * twR; m1 = t2[1] * twI; m2 = t2[1] * twR; m3 = t2[0] * twI;
    *p2++ = m0 + m1; *p2++ = m2 - m3;
    m0 = t2[3] * twI; m1 = t2[2] * twR; m2 = t2[2] * twI; m3 = t2[3] * twR;
    *pEnd2-- = m0 - m1; *pEnd2-- = m2 + m3;

    twR = tw3[0]; twI = tw3[1]; tw3 += twMod3;
    m0 = t3[0] * twR; m1 = t3[1] * twI; m2 = t3[1] * twR; m3 = t3[0] * twI;
    *p3++ = m0 + m1; *p3++ = m2 - m3;
    m0 = -t3[3] * twR; m1 = t3[2] * twI; m2 = t3[2] * twR; m3 = t3[3] * twI;
    *pEnd3-- = m0 - m1; *pEnd3-- = m3 - m2;

    twR = tw4[0]; twI = tw4[1]; tw4 += twMod4;
    m0 = t4[0] * twR; m1 = t4[1] * twI; m2 = t4[1] * twR; m3 = t4[0] * twI;
    *p4++ = m0 + m1; *p4++ = m2 - m3;
    m0 = t4[3] * twI; m1 = t4[2] * twR; m2 = t4[2] * twI; m3 = t4[3] * twR;
    *pEnd4-- = m0 - m1; *pEnd4-- = m2 + m3;
  }

  /* Middle */
  p1ap3_0 = p1[0] + p3[0]; p1sp3_0 = p1[0] - p3[0];
  p1ap3_1 = p1[1] + p3[1]; p1sp3_1 = p1[1] - p3[1];
  t2[0] = p1sp3_0 + p2[1] - p4[1];
  t2[1] = p1sp3_1 - p2[0] + p4[0];
  t3[0] = p1ap3_0 - p2[0] - p4[0];
  t3[1] = p1ap3_1 - p2[1] - p4[1];
  t4[0] = p1sp3_0 - p2[1] + p4[1];
  t4[1] = p1sp3_1 + p2[0] - p4[0];
  *p1++ = p1ap3_0 + p2[0] + p4[0];
  *p1++ = p1ap3_1 + p2[1] + p4[1];

  twR = tw2[0]; twI = tw2[1];
  m0 = t2[0] * twR; m1 = t2[1] * twI; m2 = t2[1] * twR; m3 = t2[0] * twI;
  *p2++ = m0 + m1; *p2++ = m2 - m3;

  twR = tw3[0]; twI = tw3[1];
  m0 = t3[0] * twR; m1 = t3[1] * twI; m2 = t3[1] * twR; m3 = t3[0] * twI;
  *p3++ = m0 + m1; *p3++ = m2 - m3;

  twR = tw4[0]; twI = tw4[1];
  m0 = t4[0] * twR; m1 = t4[1] * twI; m2 = t4[1] * twR; m3 = t4[0] * twI;
  *p4++ = m0 + m1; *p4++ = m2 - m3;

  arm_radix8_butterfly_f32(pCol1, L, S->pTwiddle, 4u);
  arm_radix8_butterfly_f32(pCol2, L, S->pTwiddle, 4u);
  arm_radix8_butterfly_f32(pCol3, L, S->pTwiddle, 4u);
  arm_radix8_butterfly_f32(pCol4, L, S->pTwiddle, 4u);
}

/* ----------------------------------------------------------------
 * arm_bitreversal_32 - Bit reversal for CFFT output
 * Uses byte-offset tables compatible with CMSIS format.
 * ---------------------------------------------------------------- */
static void arm_bitreversal_32(uint32_t *pSrc, const uint16_t bitRevLen, const uint16_t *pBitRevTable)
{
    uint16_t i;
    for (i = 0; i < bitRevLen; i += 2) {
        uint32_t offset1 = pBitRevTable[i];
        uint32_t offset2 = pBitRevTable[i+1];
        uint32_t *p1 = (uint32_t *)((uint8_t *)pSrc + offset1);
        uint32_t *p2 = (uint32_t *)((uint8_t *)pSrc + offset2);
        uint32_t t0 = p1[0], t1 = p1[1];
        p1[0] = p2[0]; p1[1] = p2[1];
        p2[0] = t0; p2[1] = t1;
    }
}

/* ----------------------------------------------------------------
 * Generate twiddle factors for CFFT: exp(-j*2*pi*k/N) for k=0..3*N/4-1
 * ---------------------------------------------------------------- */
static float *generate_twiddle_cfft(uint16_t n)
{
    uint32_t i;
    uint32_t len = (3u * n) / 4u;
    float *table = (float *)malloc(len * 2u * sizeof(float));
    if (!table) return 0;
    for (i = 0; i < len; i++) {
        double angle = -2.0 * 3.14159265358979323846 * (double)i / (double)n;
        table[2u * i]     = (float)cos(angle);
        table[2u * i + 1u] = (float)sin(angle);
    }
    return table;
}

/* ----------------------------------------------------------------
 * Generate byte-offset bit reversal table for CFFT
 * ---------------------------------------------------------------- */
static uint16_t *generate_bitrev_table_cfft(uint16_t n)
{
    uint32_t i, bits = 0, temp = n;
    uint16_t *table;
    while (temp > 1) { temp >>= 1; bits++; }

    /* Each pair: first = byte offset of first element to swap,
       second = byte offset of second element to swap.
       The table has bitRevLen entries. For CMSIS consistent format,
       we use pairs of indices converted to byte offsets. */
    uint32_t tblLen = 2u * n;  /* enough for the swap pairs */
    table = (uint16_t *)malloc(tblLen * sizeof(uint16_t));
    if (!table) return 0;

    uint32_t tIdx = 0;
    for (i = 0; i < n; i++) {
        uint32_t rev = bitrev_index(i, bits);
        if (rev > i) {
            table[tIdx++] = (uint16_t)(2u * i * sizeof(float));      /* byte offset of i-th complex pair */
            table[tIdx++] = (uint16_t)(2u * rev * sizeof(float));    /* byte offset of rev-th complex pair */
        }
    }

    /* Final pair: bitRevLength (number of entries, actually the last index) */
    if (tIdx < tblLen) table[tIdx] = 0;
    return table;
}

/* ----------------------------------------------------------------
 * arm_cfft_f32 - Main CFFT entry point
 * ---------------------------------------------------------------- */
void arm_cfft_f32(
    const arm_cfft_instance_f32 * S,
    float * p1,
    uint8_t ifftFlag,
    uint8_t bitReverseFlag)
{
    uint32_t L = S->fftLen, l;
    float invL, * pSrc;

    if(ifftFlag == 1u)
    {
        pSrc = p1 + 1;
        for(l=0; l<L; l++)
        {
            *pSrc = -*pSrc;
            pSrc += 2;
        }
    }

    switch (L)
    {
    case 16:
    case 128:
    case 1024:
        arm_cfft_radix8by2_f32((arm_cfft_instance_f32 *)S, p1);
        break;
    case 32:
    case 256:
    case 2048:
        arm_cfft_radix8by4_f32((arm_cfft_instance_f32 *)S, p1);
        break;
    case 64:
    case 512:
    case 4096:
        arm_radix8_butterfly_f32(p1, L, (float *)S->pTwiddle, 1);
        break;
    }

    if(bitReverseFlag)
        arm_bitreversal_32((uint32_t*)p1, 2u * L, S->pBitRevTable);

    if(ifftFlag == 1u)
    {
        invL = 1.0f/(float32_t)L;
        pSrc = p1;
        for(l=0; l<L; l++)
        {
            *pSrc++ *= invL;
            *pSrc = -(*pSrc) * invL;
            pSrc++;
        }
    }
}