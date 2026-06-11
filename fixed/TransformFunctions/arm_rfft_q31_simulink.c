/* ----------------------------------------------------------------------
 * arm_rfft_q31_simulink.c
 * Self-contained Q31 Real FFT for Simulink.
 * Uses plain C with inline q63_t operations (replaces mult_* macros).
 *
 * Based on CMSIS DSP Library arm_rfft_q31.c.
 * Calls arm_cfft_q31 (declared extern).
 * -------------------------------------------------------------------- */

#include <stdint.h>
#include <string.h>
#include <math.h>

/* --------------------------------------------------------------------
 * Type definitions
 * -------------------------------------------------------------------- */
typedef int32_t q31_t;
typedef int64_t q63_t;

/* --------------------------------------------------------------------
 * CMSIS-compatible struct types
 * -------------------------------------------------------------------- */
#ifndef ARM_CFFT_INSTANCE_Q31_DEFINED
typedef struct {
    uint32_t fftLen;
    q31_t *pTwiddle;
    uint16_t *pBitRevTable;
    uint16_t bitRevLength;
} arm_cfft_instance_q31;
#define ARM_CFFT_INSTANCE_Q31_DEFINED
#endif

#ifndef ARM_RFFT_INSTANCE_Q31_DEFINED
typedef struct {
    uint32_t fftLenReal;
    uint32_t fftLenBy2;
    uint8_t ifftFlagR;
    uint8_t bitReverseFlagR;
    uint32_t twidCoefRModifier;
    q31_t *pTwiddleAReal;
    q31_t *pTwiddleBReal;
    void *pCfft;
} arm_rfft_instance_q31;
#define ARM_RFFT_INSTANCE_Q31_DEFINED
#endif

/* --------------------------------------------------------------------
 * External declarations
 * -------------------------------------------------------------------- */
extern void arm_cfft_q31(
    const arm_cfft_instance_q31 *S,
    q31_t *p1,
    uint8_t ifftFlag,
    uint8_t bitReverseFlag);

/* --------------------------------------------------------------------
 * Forward declarations
 * -------------------------------------------------------------------- */
void arm_split_rfft_q31(
    q31_t * pSrc,
    uint32_t fftLen,
    q31_t * pATable,
    q31_t * pBTable,
    q31_t * pDst,
    uint32_t modifier);

void arm_split_rifft_q31(
    q31_t * pSrc,
    uint32_t fftLen,
    q31_t * pATable,
    q31_t * pBTable,
    q31_t * pDst,
    uint32_t modifier);

/* --------------------------------------------------------------------
 * arm_rfft_q31 - Processing function for Q31 RFFT/RIFFT
 * -------------------------------------------------------------------- */
void arm_rfft_q31(
    const arm_rfft_instance_q31 * S,
    q31_t * pSrc,
    q31_t * pDst)
{
    const arm_cfft_instance_q31 *S_CFFT = S->pCfft;
    uint32_t i;
    uint32_t L2 = S->fftLenReal >> 1;

    /* Calculation of RIFFT of input */
    if(S->ifftFlagR == 1u)
    {
        /*  Real IFFT core process */
        arm_split_rifft_q31(pSrc, L2, S->pTwiddleAReal,
                            S->pTwiddleBReal, pDst, S->twidCoefRModifier);

        /* Complex IFFT process */
        arm_cfft_q31(S_CFFT, pDst, S->ifftFlagR, S->bitReverseFlagR);

        for(i=0;i<S->fftLenReal;i++)
        {
            pDst[i] = pDst[i] << 1;
        }
    }
    else
    {
        /* Calculation of RFFT of input */

        /* Complex FFT process */
        arm_cfft_q31(S_CFFT, pSrc, S->ifftFlagR, S->bitReverseFlagR);

        /*  Real FFT core process */
        arm_split_rfft_q31(pSrc, L2, S->pTwiddleAReal,
                            S->pTwiddleBReal, pDst, S->twidCoefRModifier);
    }
}

/* --------------------------------------------------------------------
 * arm_split_rfft_q31 - Core Real FFT process
 * Uses inline q63_t operations (replaces mult_32x32_keep32_R macros)
 * -------------------------------------------------------------------- */
void arm_split_rfft_q31(
    q31_t * pSrc,
    uint32_t fftLen,
    q31_t * pATable,
    q31_t * pBTable,
    q31_t * pDst,
    uint32_t modifier)
{
    uint32_t i;                                    /* Loop Counter */
    q31_t outR, outI;                              /* Temporary variables for output */
    q31_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
    q31_t CoefA1, CoefA2, CoefB1;                  /* Temporary variables for twiddle coefficients */
    q31_t *pOut1 = &pDst[2], *pOut2 = &pDst[(4u * fftLen) - 1u];
    q31_t *pIn1 = &pSrc[2], *pIn2 = &pSrc[(2u * fftLen) - 1u];

    /* Init coefficient pointers */
    pCoefA = &pATable[modifier * 2u];
    pCoefB = &pBTable[modifier * 2u];

    i = fftLen - 1u;

    while(i > 0u)
    {
        /*
        outR = (pSrc[2 * i] * pATable[2 * i] - pSrc[2 * i + 1] * pATable[2 * i + 1]
        + pSrc[2 * n - 2 * i] * pBTable[2 * i] +
        pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);
        */

        /* outI = (pIn[2 * i + 1] * pATable[2 * i] + pIn[2 * i] * pATable[2 * i + 1] +
        pIn[2 * n - 2 * i] * pBTable[2 * i + 1] -
        pIn[2 * n - 2 * i + 1] * pBTable[2 * i]); */

        CoefA1 = *pCoefA++;
        CoefA2 = *pCoefA;

        /* outR = pSrc[2 * i] * pATable[2 * i] */
        outR = (q31_t)(((q63_t)(*pIn1) * (CoefA1) + 0x80000000LL) >> 32);

        /* outI = pIn[2 * i] * pATable[2 * i + 1] */
        outI = (q31_t)(((q63_t)(*pIn1++) * (CoefA2) + 0x80000000LL) >> 32);

        /* - pSrc[2 * i + 1] * pATable[2 * i + 1] */
        outR = (q31_t)((((q63_t)outR << 32) - (q63_t)(*pIn1) * (CoefA2) + 0x80000000LL) >> 32);

        /* pIn[2 * i + 1] * pATable[2 * i] */
        outI = (q31_t)((((q63_t)outI << 32) + (q63_t)(*pIn1++) * (CoefA1) + 0x80000000LL) >> 32);

        /* pSrc[2 * n - 2 * i] * pBTable[2 * i] */
        outR = (q31_t)((((q63_t)outR << 32) - (q63_t)(*pIn2) * (CoefA2) + 0x80000000LL) >> 32);
        CoefB1 = *pCoefB;

        /* pIn[2 * n - 2 * i] * pBTable[2 * i + 1] */
        outI = (q31_t)((((q63_t)outI << 32) - (q63_t)(*pIn2--) * (CoefB1) + 0x80000000LL) >> 32);

        /* pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1] */
        outR = (q31_t)((((q63_t)outR << 32) + (q63_t)(*pIn2) * (CoefB1) + 0x80000000LL) >> 32);

        /* pIn[2 * n - 2 * i + 1] * pBTable[2 * i] */
        outI = (q31_t)((((q63_t)outI << 32) - (q63_t)(*pIn2--) * (CoefA2) + 0x80000000LL) >> 32);

        /* write output */
        *pOut1++ = outR;
        *pOut1++ = outI;

        /* write complex conjugate output */
        *pOut2-- = -outI;
        *pOut2-- = outR;

        /* update coefficient pointer */
        pCoefB = pCoefB + (modifier * 2u);
        pCoefA = pCoefA + ((modifier * 2u) - 1u);

        i--;
    }
    pDst[2u * fftLen] = (pSrc[0] - pSrc[1]) >> 1;
    pDst[(2u * fftLen) + 1u] = 0;

    pDst[0] = (pSrc[0] + pSrc[1]) >> 1;
    pDst[1] = 0;
}

/* --------------------------------------------------------------------
 * arm_split_rifft_q31 - Core Real IFFT process
 * Uses inline q63_t operations (replaces mult_32x32_keep32_R macros)
 * -------------------------------------------------------------------- */
void arm_split_rifft_q31(
    q31_t * pSrc,
    uint32_t fftLen,
    q31_t * pATable,
    q31_t * pBTable,
    q31_t * pDst,
    uint32_t modifier)
{
    q31_t outR, outI;                              /* Temporary variables for output */
    q31_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
    q31_t CoefA1, CoefA2, CoefB1;                  /* Temporary variables for twiddle coefficients */
    q31_t *pIn1 = &pSrc[0], *pIn2 = &pSrc[(2u * fftLen) + 1u];

    pCoefA = &pATable[0];
    pCoefB = &pBTable[0];

    while(fftLen > 0u)
    {
        /*
        outR = (pIn[2 * i] * pATable[2 * i] + pIn[2 * i + 1] * pATable[2 * i + 1] +
        pIn[2 * n - 2 * i] * pBTable[2 * i] -
        pIn[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);

        outI = (pIn[2 * i + 1] * pATable[2 * i] - pIn[2 * i] * pATable[2 * i + 1] -
        pIn[2 * n - 2 * i] * pBTable[2 * i + 1] -
        pIn[2 * n - 2 * i + 1] * pBTable[2 * i]);
        */
        CoefA1 = *pCoefA++;
        CoefA2 = *pCoefA;

        /* outR = pIn[2 * i] * pATable[2 * i] */
        outR = (q31_t)(((q63_t)(*pIn1) * (CoefA1) + 0x80000000LL) >> 32);

        /* - pIn[2 * i] * pATable[2 * i + 1] */
        outI = (q31_t)(((q63_t)(*pIn1++) * (-CoefA2) + 0x80000000LL) >> 32);

        /* pIn[2 * i + 1] * pATable[2 * i + 1] */
        outR = (q31_t)((((q63_t)outR << 32) + (q63_t)(*pIn1) * (CoefA2) + 0x80000000LL) >> 32);

        /* pIn[2 * i + 1] * pATable[2 * i] */
        outI = (q31_t)((((q63_t)outI << 32) + (q63_t)(*pIn1++) * (CoefA1) + 0x80000000LL) >> 32);

        /* pIn[2 * n - 2 * i] * pBTable[2 * i] */
        outR = (q31_t)((((q63_t)outR << 32) + (q63_t)(*pIn2) * (CoefA2) + 0x80000000LL) >> 32);
        CoefB1 = *pCoefB;

        /* pIn[2 * n - 2 * i] * pBTable[2 * i + 1] */
        outI = (q31_t)((((q63_t)outI << 32) - (q63_t)(*pIn2--) * (CoefB1) + 0x80000000LL) >> 32);

        /* pIn[2 * n - 2 * i + 1] * pBTable[2 * i + 1] */
        outR = (q31_t)((((q63_t)outR << 32) + (q63_t)(*pIn2) * (CoefB1) + 0x80000000LL) >> 32);

        /* pIn[2 * n - 2 * i + 1] * pBTable[2 * i] */
        outI = (q31_t)((((q63_t)outI << 32) + (q63_t)(*pIn2--) * (CoefA2) + 0x80000000LL) >> 32);

        /* write output */
        *pDst++ = outR;
        *pDst++ = outI;

        /* update coefficient pointer */
        pCoefB = pCoefB + (modifier * 2u);
        pCoefA = pCoefA + ((modifier * 2u) - 1u);

        /* Decrement loop count */
        fftLen--;
    }
}