#include <stdint.h>

typedef int16_t q15_t;
typedef int32_t q31_t;

typedef struct {
    uint16_t fftLen;
    uint8_t  ifftFlag;
    uint8_t  bitReverseFlag;
    uint16_t twidCoefModifier;
    uint16_t bitRevFactor;
    q15_t   *pTwiddle;
    uint16_t *pBitRevTable;
} arm_cfft_radix4_instance_q15;

static int16_t ssat16(int32_t val) {
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (int16_t)val;
}

void arm_bitreversal_q15(q15_t *pSrc16, uint32_t fftLen, uint16_t bitRevFactor, uint16_t *pBitRevTab)
{
    q31_t *pSrc = (q31_t *)pSrc16;
    q31_t in;
    uint32_t fftLenBy2, fftLenBy2p1, i, j;

    j = 0u;
    fftLenBy2 = fftLen / 2u;
    fftLenBy2p1 = (fftLen / 2u) + 1u;
    for (i = 0u; i <= (fftLenBy2 - 2u); i += 2u) {
        if (i < j) {
            in = pSrc[i]; pSrc[i] = pSrc[j]; pSrc[j] = in;
            in = pSrc[i + fftLenBy2p1]; pSrc[i + fftLenBy2p1] = pSrc[j + fftLenBy2p1]; pSrc[j + fftLenBy2p1] = in;
        }
        in = pSrc[i + 1u]; pSrc[i + 1u] = pSrc[j + fftLenBy2]; pSrc[j + fftLenBy2] = in;
        j = *pBitRevTab;
        pBitRevTab += bitRevFactor;
    }
}

void arm_radix4_butterfly_q15(
  q15_t * pSrc16,
  uint32_t fftLen,
  q15_t * pCoef16,
  uint32_t twidCoefModifier)
{

  /* Run the below code for Cortex-M0 */

  q15_t R0, R1, S0, S1, T0, T1, U0, U1;
  q15_t Co1, Si1, Co2, Si2, Co3, Si3, out1, out2;
  uint32_t n1, n2, ic, i0, i1, i2, i3, j, k;

  /* Total process is divided into three stages */

  /* process first stage, middle stages, & last stage */

  /*  Initializations for the first stage */
  n2 = fftLen;
  n1 = n2;

  /* n2 = fftLen/4 */
  n2 >>= 2u;

  /* Index for twiddle coefficient */
  ic = 0u;

  /* Index for input read and output write */
  i0 = 0u;
  j = n2;

  /* Input is in 1.15(q15) format */

  /*  start of first stage process */
  do
  {
    /*  Butterfly implementation */

    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */

    /* input is down scale by 4 to avoid overflow */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u] >> 2u;
    T1 = pSrc16[(i0 * 2u) + 1u] >> 2u;

    /* input is down scale by 4 to avoid overflow */
    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u] >> 2u;
    S1 = pSrc16[(i2 * 2u) + 1u] >> 2u;

    /* R0 = (ya + yc) */
    R0 = ssat16((int32_t)T0 + S0);
    /* R1 = (xa + xc) */
    R1 = ssat16((int32_t)T1 + S1);

    /* S0 = (ya - yc) */
    S0 = ssat16((int32_t)T0 - S0);
    /* S1 = (xa - xc) */
    S1 = ssat16((int32_t)T1 - S1);

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* input is down scale by 4 to avoid overflow */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u] >> 2u;
    T1 = pSrc16[(i1 * 2u) + 1u] >> 2u;

    /* input is down scale by 4 to avoid overflow */
    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u] >> 2u;
    U1 = pSrc16[(i3 * 2u) + 1] >> 2u;

    /* T0 = (yb + yd) */
    T0 = ssat16((int32_t)T0 + U0);
    /* T1 = (xb + xd) */
    T1 = ssat16((int32_t)T1 + U1);

    /*  writing the butterfly processed i0 sample */
    /* ya' = ya + yb + yc + yd */
    /* xa' = xa + xb + xc + xd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd) */
    /* R1 = (xa + xc) - (xb + xd) */
    R0 = ssat16((int32_t)R0 - T0);
    R1 = ssat16((int32_t)R1 - T1);

    /* co2 & si2 are read from Coefficient pointer */
    Co2 = pCoef16[2u * ic * 2u];
    Si2 = pCoef16[(2u * ic * 2u) + 1];

    /* xc' = (xa-xb+xc-xd)* co2 + (ya-yb+yc-yd)* (si2) */
    out1 = (q15_t) ((Co2 * R0 + Si2 * R1) >> 16u);
    /* yc' = (ya-yb+yc-yd)* co2 - (xa-xb+xc-xd)* (si2) */
    out2 = (q15_t) ((-Si2 * R0 + Co2 * R1) >> 16u);

    /*  Reading i0+fftLen/4 */
    /* input is down scale by 4 to avoid overflow */
    /* T0 = yb, T1 =  xb */
    T0 = pSrc16[i1 * 2u] >> 2;
    T1 = pSrc16[(i1 * 2u) + 1] >> 2;

    /* writing the butterfly processed i0 + fftLen/4 sample */
    /* writing output(xc', yc') in little endian format */
    pSrc16[i1 * 2u] = out1;
    pSrc16[(i1 * 2u) + 1] = out2;

    /*  Butterfly calculations */
    /* input is down scale by 4 to avoid overflow */
    /* U0 = yd, U1 = xd */
    U0 = pSrc16[i3 * 2u] >> 2;
    U1 = pSrc16[(i3 * 2u) + 1] >> 2;
    /* T0 = yb-yd */
    T0 = ssat16((int32_t)T0 - U0);
    /* T1 = xb-xd */
    T1 = ssat16((int32_t)T1 - U1);

    /* R1 = (ya-yc) + (xb- xd),  R0 = (xa-xc) - (yb-yd)) */
    R0 = ssat16((int32_t)S0 - (int32_t)T1);
    R1 = ssat16((int32_t)S1 + (int32_t)T0);

    /* S1 = (ya-yc) - (xb- xd), S0 = (xa-xc) + (yb-yd)) */
    S0 = ssat16((int32_t)S0 + (int32_t)T1);
    S1 = ssat16((int32_t)S1 - (int32_t)T0);

    /* co1 & si1 are read from Coefficient pointer */
    Co1 = pCoef16[ic * 2u];
    Si1 = pCoef16[(ic * 2u) + 1];
    /*  Butterfly process for the i0+fftLen/2 sample */
    /* xb' = (xa+yb-xc-yd)* co1 + (ya-xb-yc+xd)* (si1) */
    out1 = (q15_t) ((Si1 * S1 + Co1 * S0) >> 16);
    /* yb' = (ya-xb-yc+xd)* co1 - (xa+yb-xc-yd)* (si1) */
    out2 = (q15_t) ((-Si1 * S0 + Co1 * S1) >> 16);

    /* writing output(xb', yb') in little endian format */
    pSrc16[i2 * 2u] = out1;
    pSrc16[(i2 * 2u) + 1] = out2;

    /* Co3 & si3 are read from Coefficient pointer */
    Co3 = pCoef16[3u * (ic * 2u)];
    Si3 = pCoef16[(3u * (ic * 2u)) + 1];
    /*  Butterfly process for the i0+3fftLen/4 sample */
    /* xd' = (xa-yb-xc+yd)* Co3 + (ya+xb-yc-xd)* (si3) */
    out1 = (q15_t) ((Si3 * R1 + Co3 * R0) >> 16u);
    /* yd' = (ya+xb-yc-xd)* Co3 - (xa-yb-xc+yd)* (si3) */
    out2 = (q15_t) ((-Si3 * R0 + Co3 * R1) >> 16u);
    /* writing output(xd', yd') in little endian format */
    pSrc16[i3 * 2u] = out1;
    pSrc16[(i3 * 2u) + 1] = out2;

    /*  Twiddle coefficients index modifier */
    ic = ic + twidCoefModifier;

    /*  Updating input index */
    i0 = i0 + 1u;

  } while(--j);
  /* data is in 4.11(q11) format */

  /* end of first stage process */


  /* start of middle stage process */

  /*  Twiddle coefficients index modifier */
  twidCoefModifier <<= 2u;

  /*  Calculation of Middle stage */
  for (k = fftLen / 4u; k > 4u; k >>= 2u)
  {
    /*  Initializations for the middle stage */
    n1 = n2;
    n2 >>= 2u;
    ic = 0u;

    for (j = 0u; j <= (n2 - 1u); j++)
    {
      /*  index calculation for the coefficients */
      Co1 = pCoef16[ic * 2u];
      Si1 = pCoef16[(ic * 2u) + 1u];
      Co2 = pCoef16[2u * (ic * 2u)];
      Si2 = pCoef16[(2u * (ic * 2u)) + 1u];
      Co3 = pCoef16[3u * (ic * 2u)];
      Si3 = pCoef16[(3u * (ic * 2u)) + 1u];

      /*  Twiddle coefficients index modifier */
      ic = ic + twidCoefModifier;

      /*  Butterfly implementation */
      for (i0 = j; i0 < fftLen; i0 += n1)
      {
        /*  index calculation for the input as, */
        /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
        i1 = i0 + n2;
        i2 = i1 + n2;
        i3 = i2 + n2;

        /*  Reading i0, i0+fftLen/2 inputs */
        /* Read ya (real), xa(imag) input */
        T0 = pSrc16[i0 * 2u];
        T1 = pSrc16[(i0 * 2u) + 1u];

        /* Read yc (real), xc(imag) input */
        S0 = pSrc16[i2 * 2u];
        S1 = pSrc16[(i2 * 2u) + 1u];

        /* R0 = (ya + yc), R1 = (xa + xc) */
        R0 = ssat16((int32_t)T0 + S0);
        R1 = ssat16((int32_t)T1 + S1);

        /* S0 = (ya - yc), S1 =(xa - xc) */
        S0 = ssat16((int32_t)T0 - S0);
        S1 = ssat16((int32_t)T1 - S1);

        /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];


        /* T0 = (yb + yd), T1 = (xb + xd) */
        T0 = ssat16((int32_t)T0 + U0);
        T1 = ssat16((int32_t)T1 + U1);

        /*  writing the butterfly processed i0 sample */

        /* xa' = xa + xb + xc + xd */
        /* ya' = ya + yb + yc + yd */
        out1 = ((R0 >> 1u) + (T0 >> 1u)) >> 1u;
        out2 = ((R1 >> 1u) + (T1 >> 1u)) >> 1u;

        pSrc16[i0 * 2u] = out1;
        pSrc16[(2u * i0) + 1u] = out2;

        /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
        R0 = (R0 >> 1u) - (T0 >> 1u);
        R1 = (R1 >> 1u) - (T1 >> 1u);

        /* (ya-yb+yc-yd)* (si2) + (xa-xb+xc-xd)* co2 */
        out1 = (q15_t) ((Co2 * R0 + Si2 * R1) >> 16u);

        /* (ya-yb+yc-yd)* co2 - (xa-xb+xc-xd)* (si2) */
        out2 = (q15_t) ((-Si2 * R0 + Co2 * R1) >> 16u);

        /*  Reading i0+3fftLen/4 */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /*  writing the butterfly processed i0 + fftLen/4 sample */
        /* xc' = (xa-xb+xc-xd)* co2 + (ya-yb+yc-yd)* (si2) */
        /* yc' = (ya-yb+yc-yd)* co2 - (xa-xb+xc-xd)* (si2) */
        pSrc16[i1 * 2u] = out1;
        pSrc16[(i1 * 2u) + 1u] = out2;

        /*  Butterfly calculations */

        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];

        /* T0 = yb-yd, T1 = xb-xd */
        T0 = ssat16((int32_t)T0 - U0);
        T1 = ssat16((int32_t)T1 - U1);

        /* R0 = (ya-yc) + (xb- xd), R1 = (xa-xc) - (yb-yd)) */
        R0 = (S0 >> 1u) - (T1 >> 1u);
        R1 = (S1 >> 1u) + (T0 >> 1u);

        /* S0 = (ya-yc) - (xb- xd), S1 = (xa-xc) + (yb-yd)) */
        S0 = (S0 >> 1u) + (T1 >> 1u);
        S1 = (S1 >> 1u) - (T0 >> 1u);

        /*  Butterfly process for the i0+fftLen/2 sample */
        out1 = (q15_t) ((Co1 * S0 + Si1 * S1) >> 16u);

        out2 = (q15_t) ((-Si1 * S0 + Co1 * S1) >> 16u);

        /* xb' = (xa+yb-xc-yd)* co1 + (ya-xb-yc+xd)* (si1) */
        /* yb' = (ya-xb-yc+xd)* co1 - (xa+yb-xc-yd)* (si1) */
        pSrc16[i2 * 2u] = out1;
        pSrc16[(i2 * 2u) + 1u] = out2;

        /*  Butterfly process for the i0+3fftLen/4 sample */
        out1 = (q15_t) ((Si3 * R1 + Co3 * R0) >> 16u);

        out2 = (q15_t) ((-Si3 * R0 + Co3 * R1) >> 16u);
        /* xd' = (xa-yb-xc+yd)* Co3 + (ya+xb-yc-xd)* (si3) */
        /* yd' = (ya+xb-yc-xd)* Co3 - (xa-yb-xc+yd)* (si3) */
        pSrc16[i3 * 2u] = out1;
        pSrc16[(i3 * 2u) + 1u] = out2;
      }
    }
    /*  Twiddle coefficients index modifier */
    twidCoefModifier <<= 2u;
  }
  /* end of middle stage process */


  /* data is in 10.6(q6) format for the 1024 point */
  /* data is in 8.8(q8) format for the 256 point */
  /* data is in 6.10(q10) format for the 64 point */
  /* data is in 4.12(q12) format for the 16 point */

  /*  Initializations for the last stage */
  n1 = n2;
  n2 >>= 2u;

  /* start of last stage process */

  /*  Butterfly implementation */
  for (i0 = 0u; i0 <= (fftLen - n1); i0 += n1)
  {
    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u];
    T1 = pSrc16[(i0 * 2u) + 1u];

    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u];
    S1 = pSrc16[(i2 * 2u) + 1u];

    /* R0 = (ya + yc), R1 = (xa + xc) */
    R0 = ssat16((int32_t)T0 + S0);
    R1 = ssat16((int32_t)T1 + S1);

    /* S0 = (ya - yc), S1 = (xa - xc) */
    S0 = ssat16((int32_t)T0 - S0);
    S1 = ssat16((int32_t)T1 - S1);

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];
    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];

    /* T0 = (yb + yd), T1 = (xb + xd)) */
    T0 = ssat16((int32_t)T0 + U0);
    T1 = ssat16((int32_t)T1 + U1);

    /*  writing the butterfly processed i0 sample */
    /* xa' = xa + xb + xc + xd */
    /* ya' = ya + yb + yc + yd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
    R0 = (R0 >> 1u) - (T0 >> 1u);
    R1 = (R1 >> 1u) - (T1 >> 1u);

    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];

    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];
    /* T0 = (yb - yd), T1 = (xb - xd)  */
    T0 = ssat16((int32_t)T0 - U0);
    T1 = ssat16((int32_t)T1 - U1);

    /*  writing the butterfly processed i0 + fftLen/4 sample */
    /* xb' = (xa+yb-xc-yd) */
    /* yb' = (ya-xb-yc+xd) */
    pSrc16[i1 * 2u] = (S0 >> 1u) + (T1 >> 1u);
    pSrc16[(i1 * 2u) + 1u] = (S1 >> 1u) - (T0 >> 1u);

    /*  writing the butterfly processed i0 + fftLen/2 sample */
    /* xc' = (xa-xb+xc-xd) */
    /* yc' = (ya-yb+yc-yd) */
    pSrc16[i2 * 2u] = R0;
    pSrc16[(i2 * 2u) + 1u] = R1;

    /*  writing the butterfly processed i0 + 3fftLen/4 sample */
    /* xd' = (xa-yb-xc+yd) */
    /* yd' = (ya+xb-yc-xd) */
    pSrc16[i3 * 2u] = (S0 >> 1u) - (T1 >> 1u);
    pSrc16[(i3 * 2u) + 1u] = (S1 >> 1u) + (T0 >> 1u);

  }

  /* end of last stage process */

  /* output is in 11.5(q5) format for the 1024 point */
  /* output is in 9.7(q7) format for the 256 point   */
  /* output is in 7.9(q9) format for the 64 point  */
  /* output is in 5.11(q11) format for the 16 point  */

}


void arm_radix4_butterfly_inverse_q15(
  q15_t * pSrc16,
  uint32_t fftLen,
  q15_t * pCoef16,
  uint32_t twidCoefModifier)
{

  /* Run the below code for Cortex-M0 */

  q15_t R0, R1, S0, S1, T0, T1, U0, U1;
  q15_t Co1, Si1, Co2, Si2, Co3, Si3, out1, out2;
  uint32_t n1, n2, ic, i0, i1, i2, i3, j, k;

  /* Total process is divided into three stages */

  /* process first stage, middle stages, & last stage */

  /*  Initializations for the first stage */
  n2 = fftLen;
  n1 = n2;

  /* n2 = fftLen/4 */
  n2 >>= 2u;

  /* Index for twiddle coefficient */
  ic = 0u;

  /* Index for input read and output write */
  i0 = 0u;

  j = n2;

  /* Input is in 1.15(q15) format */

  /*  Start of first stage process */
  do
  {
    /*  Butterfly implementation */

    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */
    /* input is down scale by 4 to avoid overflow */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u] >> 2u;
    T1 = pSrc16[(i0 * 2u) + 1u] >> 2u;
    /* input is down scale by 4 to avoid overflow */
    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u] >> 2u;
    S1 = pSrc16[(i2 * 2u) + 1u] >> 2u;

    /* R0 = (ya + yc), R1 = (xa + xc) */
    R0 = ssat16((int32_t)T0 + S0);
    R1 = ssat16((int32_t)T1 + S1);
    /* S0 = (ya - yc), S1 = (xa - xc) */
    S0 = ssat16((int32_t)T0 - S0);
    S1 = ssat16((int32_t)T1 - S1);

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* input is down scale by 4 to avoid overflow */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u] >> 2u;
    T1 = pSrc16[(i1 * 2u) + 1u] >> 2u;
    /* Read yd (real), xd(imag) input */
    /* input is down scale by 4 to avoid overflow */
    U0 = pSrc16[i3 * 2u] >> 2u;
    U1 = pSrc16[(i3 * 2u) + 1u] >> 2u;

    /* T0 = (yb + yd), T1 = (xb + xd) */
    T0 = ssat16((int32_t)T0 + U0);
    T1 = ssat16((int32_t)T1 + U1);

    /*  writing the butterfly processed i0 sample */
    /* xa' = xa + xb + xc + xd */
    /* ya' = ya + yb + yc + yd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc)- (xb + xd) */
    R0 = ssat16((int32_t)R0 - T0);
    R1 = ssat16((int32_t)R1 - T1);
    /* co2 & si2 are read from Coefficient pointer */
    Co2 = pCoef16[2u * ic * 2u];
    Si2 = pCoef16[(2u * ic * 2u) + 1u];
    /* xc' = (xa-xb+xc-xd)* co2 - (ya-yb+yc-yd)* (si2) */
    out1 = (q15_t) ((Co2 * R0 - Si2 * R1) >> 16u);
    /* yc' = (ya-yb+yc-yd)* co2 + (xa-xb+xc-xd)* (si2) */
    out2 = (q15_t) ((Si2 * R0 + Co2 * R1) >> 16u);

    /*  Reading i0+fftLen/4 */
    /* input is down scale by 4 to avoid overflow */
    /* T0 = yb, T1 = xb */
    T0 = pSrc16[i1 * 2u] >> 2u;
    T1 = pSrc16[(i1 * 2u) + 1u] >> 2u;

    /* writing the butterfly processed i0 + fftLen/4 sample */
    /* writing output(xc', yc') in little endian format */
    pSrc16[i1 * 2u] = out1;
    pSrc16[(i1 * 2u) + 1u] = out2;

    /*  Butterfly calculations */
    /* input is down scale by 4 to avoid overflow */
    /* U0 = yd, U1 = xd) */
    U0 = pSrc16[i3 * 2u] >> 2u;
    U1 = pSrc16[(i3 * 2u) + 1u] >> 2u;

    /* T0 = yb-yd, T1 = xb-xd) */
    T0 = ssat16((int32_t)T0 - U0);
    T1 = ssat16((int32_t)T1 - U1);
    /* R0 = (ya-yc) - (xb- xd) , R1 = (xa-xc) + (yb-yd) */
    R0 = ssat16((int32_t)S0 + (int32_t)T1);
    R1 = ssat16((int32_t)S1 - (int32_t)T0);
    /* S = (ya-yc) + (xb- xd), S1 = (xa-xc) - (yb-yd) */
    S0 = ssat16((int32_t)S0 - (int32_t)T1);
    S1 = ssat16((int32_t)S1 + (int32_t)T0);

    /* co1 & si1 are read from Coefficient pointer */
    Co1 = pCoef16[ic * 2u];
    Si1 = pCoef16[(ic * 2u) + 1u];
    /*  Butterfly process for the i0+fftLen/2 sample */
    /* xb' = (xa-yb-xc+yd)* co1 - (ya+xb-yc-xd)* (si1) */
    out1 = (q15_t) ((Co1 * S0 - Si1 * S1) >> 16u);
    /* yb' = (ya+xb-yc-xd)* co1 + (xa-yb-xc+yd)* (si1) */
    out2 = (q15_t) ((Si1 * S0 + Co1 * S1) >> 16u);
    /* writing output(xb', yb') in little endian format */
    pSrc16[i2 * 2u] = out1;
    pSrc16[(i2 * 2u) + 1u] = out2;

    /* Co3 & si3 are read from Coefficient pointer */
    Co3 = pCoef16[3u * ic * 2u];
    Si3 = pCoef16[(3u * ic * 2u) + 1u];
    /*  Butterfly process for the i0+3fftLen/4 sample */
    /* xd' = (xa+yb-xc-yd)* Co3 - (ya-xb-yc+xd)* (si3) */
    out1 = (q15_t) ((Co3 * R0 - Si3 * R1) >> 16u);
    /* yd' = (ya-xb-yc+xd)* Co3 + (xa+yb-xc-yd)* (si3) */
    out2 = (q15_t) ((Si3 * R0 + Co3 * R1) >> 16u);
    /* writing output(xd', yd') in little endian format */
    pSrc16[i3 * 2u] = out1;
    pSrc16[(i3 * 2u) + 1u] = out2;

    /*  Twiddle coefficients index modifier */
    ic = ic + twidCoefModifier;

    /*  Updating input index */
    i0 = i0 + 1u;

  } while(--j);

  /*  End of first stage process */

  /* data is in 4.11(q11) format */


  /*  Start of Middle stage process */

  /*  Twiddle coefficients index modifier */
  twidCoefModifier <<= 2u;

  /*  Calculation of Middle stage */
  for (k = fftLen / 4u; k > 4u; k >>= 2u)
  {
    /*  Initializations for the middle stage */
    n1 = n2;
    n2 >>= 2u;
    ic = 0u;

    for (j = 0u; j <= (n2 - 1u); j++)
    {
      /*  index calculation for the coefficients */
      Co1 = pCoef16[ic * 2u];
      Si1 = pCoef16[(ic * 2u) + 1u];
      Co2 = pCoef16[2u * ic * 2u];
      Si2 = pCoef16[2u * ic * 2u + 1u];
      Co3 = pCoef16[3u * ic * 2u];
      Si3 = pCoef16[(3u * ic * 2u) + 1u];

      /*  Twiddle coefficients index modifier */
      ic = ic + twidCoefModifier;

      /*  Butterfly implementation */
      for (i0 = j; i0 < fftLen; i0 += n1)
      {
        /*  index calculation for the input as, */
        /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
        i1 = i0 + n2;
        i2 = i1 + n2;
        i3 = i2 + n2;

        /*  Reading i0, i0+fftLen/2 inputs */
        /* Read ya (real), xa(imag) input */
        T0 = pSrc16[i0 * 2u];
        T1 = pSrc16[(i0 * 2u) + 1u];

        /* Read yc (real), xc(imag) input */
        S0 = pSrc16[i2 * 2u];
        S1 = pSrc16[(i2 * 2u) + 1u];


        /* R0 = (ya + yc), R1 = (xa + xc) */
        R0 = ssat16((int32_t)T0 + S0);
        R1 = ssat16((int32_t)T1 + S1);
        /* S0 = (ya - yc), S1 = (xa - xc) */
        S0 = ssat16((int32_t)T0 - S0);
        S1 = ssat16((int32_t)T1 - S1);

        /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];

        /* T0 = (yb + yd), T1 = (xb + xd) */
        T0 = ssat16((int32_t)T0 + U0);
        T1 = ssat16((int32_t)T1 + U1);

        /*  writing the butterfly processed i0 sample */
        /* xa' = xa + xb + xc + xd */
        /* ya' = ya + yb + yc + yd */
        pSrc16[i0 * 2u] = ((R0 >> 1u) + (T0 >> 1u)) >> 1u;
        pSrc16[(i0 * 2u) + 1u] = ((R1 >> 1u) + (T1 >> 1u)) >> 1u;

        /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
        R0 = (R0 >> 1u) - (T0 >> 1u);
        R1 = (R1 >> 1u) - (T1 >> 1u);

        /* (ya-yb+yc-yd)* (si2) - (xa-xb+xc-xd)* co2 */
        out1 = (q15_t) ((Co2 * R0 - Si2 * R1) >> 16);
        /* (ya-yb+yc-yd)* co2 + (xa-xb+xc-xd)* (si2) */
        out2 = (q15_t) ((Si2 * R0 + Co2 * R1) >> 16);

        /*  Reading i0+3fftLen/4 */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /*  writing the butterfly processed i0 + fftLen/4 sample */
        /* xc' = (xa-xb+xc-xd)* co2 - (ya-yb+yc-yd)* (si2) */
        /* yc' = (ya-yb+yc-yd)* co2 + (xa-xb+xc-xd)* (si2) */
        pSrc16[i1 * 2u] = out1;
        pSrc16[(i1 * 2u) + 1u] = out2;

        /*  Butterfly calculations */
        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];

        /* T0 = yb-yd, T1 = xb-xd) */
        T0 = ssat16((int32_t)T0 - U0);
        T1 = ssat16((int32_t)T1 - U1);

        /* R0 = (ya-yc) - (xb- xd) , R1 = (xa-xc) + (yb-yd) */
        R0 = (S0 >> 1u) + (T1 >> 1u);
        R1 = (S1 >> 1u) - (T0 >> 1u);

        /* S1 = (ya-yc) + (xb- xd), S1 = (xa-xc) - (yb-yd) */
        S0 = (S0 >> 1u) - (T1 >> 1u);
        S1 = (S1 >> 1u) + (T0 >> 1u);

        /*  Butterfly process for the i0+fftLen/2 sample */
        out1 = (q15_t) ((Co1 * S0 - Si1 * S1) >> 16u);
        out2 = (q15_t) ((Si1 * S0 + Co1 * S1) >> 16u);
        /* xb' = (xa-yb-xc+yd)* co1 - (ya+xb-yc-xd)* (si1) */
        /* yb' = (ya+xb-yc-xd)* co1 + (xa-yb-xc+yd)* (si1) */
        pSrc16[i2 * 2u] = out1;
        pSrc16[(i2 * 2u) + 1u] = out2;

        /*  Butterfly process for the i0+3fftLen/4 sample */
        out1 = (q15_t) ((Co3 * R0 - Si3 * R1) >> 16u);

        out2 = (q15_t) ((Si3 * R0 + Co3 * R1) >> 16u);
        /* xd' = (xa+yb-xc-yd)* Co3 - (ya-xb-yc+xd)* (si3) */
        /* yd' = (ya-xb-yc+xd)* Co3 + (xa+yb-xc-yd)* (si3) */
        pSrc16[i3 * 2u] = out1;
        pSrc16[(i3 * 2u) + 1u] = out2;


      }
    }
    /*  Twiddle coefficients index modifier */
    twidCoefModifier <<= 2u;
  }
  /*  End of Middle stages process */


  /* data is in 10.6(q6) format for the 1024 point */
  /* data is in 8.8(q8) format for the 256 point   */
  /* data is in 6.10(q10) format for the 64 point  */
  /* data is in 4.12(q12) format for the 16 point  */

  /* start of last stage process */


  /*  Initializations for the last stage */
  n1 = n2;
  n2 >>= 2u;

  /*  Butterfly implementation */
  for (i0 = 0u; i0 <= (fftLen - n1); i0 += n1)
  {
    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u];
    T1 = pSrc16[(i0 * 2u) + 1u];
    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u];
    S1 = pSrc16[(i2 * 2u) + 1u];

    /* R0 = (ya + yc), R1 = (xa + xc) */
    R0 = ssat16((int32_t)T0 + S0);
    R1 = ssat16((int32_t)T1 + S1);
    /* S0 = (ya - yc), S1 = (xa - xc) */
    S0 = ssat16((int32_t)T0 - S0);
    S1 = ssat16((int32_t)T1 - S1);

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];
    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];

    /* T0 = (yb + yd), T1 = (xb + xd) */
    T0 = ssat16((int32_t)T0 + U0);
    T1 = ssat16((int32_t)T1 + U1);

    /*  writing the butterfly processed i0 sample */
    /* xa' = xa + xb + xc + xd */
    /* ya' = ya + yb + yc + yd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
    R0 = (R0 >> 1u) - (T0 >> 1u);
    R1 = (R1 >> 1u) - (T1 >> 1u);

    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];

    /*  writing the butterfly processed i0 + fftLen/4 sample */
    /* xb' = (xa-yb-xc+yd) */
    /* yb' = (ya+xb-yc-xd) */
    pSrc16[i1 * 2u] = (S0 >> 1u) - (T1 >> 1u);
    pSrc16[(i1 * 2u) + 1u] = (S1 >> 1u) + (T0 >> 1u);

    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];
    /* T0 = (yb - yd), T1 = (xb - xd) */
    T0 = ssat16((int32_t)T0 - U0);
    T1 = ssat16((int32_t)T1 - U1);

    /*  writing the butterfly processed i0 + fftLen/2 sample */
    /* xc' = (xa-xb+xc-xd) */
    /* yc' = (ya-yb+yc-yd) */
    pSrc16[i2 * 2u] = R0;
    pSrc16[(i2 * 2u) + 1u] = R1;


    /*  writing the butterfly processed i0 + 3fftLen/4 sample */
    /* xd' = (xa+yb-xc-yd) */
    /* yd' = (ya-xb-yc+xd) */
    pSrc16[i3 * 2u] = (S0 >> 1u) + (T1 >> 1u);
    pSrc16[(i3 * 2u) + 1u] = (S1 >> 1u) - (T0 >> 1u);
  }
  /* end of last stage  process */

  /* output is in 11.5(q5) format for the 1024 point */
  /* output is in 9.7(q7) format for the 256 point   */
  /* output is in 7.9(q9) format for the 64 point  */
  /* output is in 5.11(q11) format for the 16 point  */

}


void arm_cfft_radix4_q15(
  const arm_cfft_radix4_instance_q15 * S,
  q15_t * pSrc)
{
  if(S->ifftFlag == 1u)
  {
    /*  Complex IFFT radix-4  */
    arm_radix4_butterfly_inverse_q15(pSrc, S->fftLen, S->pTwiddle,
                                     S->twidCoefModifier);
  }
  else
  {
    /*  Complex FFT radix-4  */
    arm_radix4_butterfly_q15(pSrc, S->fftLen, S->pTwiddle,
                             S->twidCoefModifier);
  }

  if(S->bitReverseFlag == 1u)
  {
    /*  Bit Reversal */
    arm_bitreversal_q15(pSrc, S->fftLen, S->bitRevFactor, S->pBitRevTable);
  }

}