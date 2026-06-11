/* ----------------------------------------------------------------------
 * arm_rfft_q15_simulink.c
 * Self-contained Q15 Real FFT for Simulink.
 * Uses CM0-only (plain C) code paths from CMSIS DSP Library.
 *
 * Based on CMSIS DSP Library arm_rfft_q15.c (CM0 paths only).
 * Calls arm_cfft_q15 (declared extern).
 * -------------------------------------------------------------------- */

#include <stdint.h>
#include <string.h>
#include <math.h>

/* --------------------------------------------------------------------
 * Type definitions
 * -------------------------------------------------------------------- */
typedef int16_t q15_t;
typedef int32_t q31_t;

/* --------------------------------------------------------------------
 * CMSIS-compatible struct types
 * -------------------------------------------------------------------- */
#ifndef ARM_CFFT_INSTANCE_Q15_DEFINED
typedef struct {
    uint32_t fftLen;
    q15_t *pTwiddle;
    uint16_t *pBitRevTable;
    uint16_t bitRevLength;
} arm_cfft_instance_q15;
#define ARM_CFFT_INSTANCE_Q15_DEFINED
#endif

#ifndef ARM_RFFT_INSTANCE_Q15_DEFINED
typedef struct {
    uint32_t fftLenReal;
    uint32_t fftLenBy2;
    uint8_t ifftFlagR;
    uint8_t bitReverseFlagR;
    uint32_t twidCoefRModifier;
    q15_t *pTwiddleAReal;
    q15_t *pTwiddleBReal;
    void *pCfft;
} arm_rfft_instance_q15;
#define ARM_RFFT_INSTANCE_Q15_DEFINED
#endif

/* --------------------------------------------------------------------
 * External declarations
 * -------------------------------------------------------------------- */
extern void arm_cfft_q15(
    const arm_cfft_instance_q15 *S,
    q15_t *p1,
    uint8_t ifftFlag,
    uint8_t bitReverseFlag);

/* --------------------------------------------------------------------
 * Forward declarations
 * -------------------------------------------------------------------- */
void arm_split_rfft_q15(
    q15_t * pSrc,
    uint32_t fftLen,
    q15_t * pATable,
    q15_t * pBTable,
    q15_t * pDst,
    uint32_t modifier);

void arm_split_rifft_q15(
    q15_t * pSrc,
    uint32_t fftLen,
    q15_t * pATable,
    q15_t * pBTable,
    q15_t * pDst,
    uint32_t modifier);

/* --------------------------------------------------------------------
 * arm_rfft_q15 - Processing function for Q15 RFFT/RIFFT
 * -------------------------------------------------------------------- */
void arm_rfft_q15(
    const arm_rfft_instance_q15 * S,
    q15_t * pSrc,
    q15_t * pDst)
{
    const arm_cfft_instance_q15 *S_CFFT = S->pCfft;
    uint32_t i;
    uint32_t L2 = S->fftLenReal >> 1;

    /* Calculation of RIFFT of input */
    if(S->ifftFlagR == 1u)
    {
        /*  Real IFFT core process */
        arm_split_rifft_q15(pSrc, L2, S->pTwiddleAReal,
                            S->pTwiddleBReal, pDst, S->twidCoefRModifier);

        /* Complex IFFT process */
        arm_cfft_q15(S_CFFT, pDst, S->ifftFlagR, S->bitReverseFlagR);

        for(i=0;i<S->fftLenReal;i++)
        {
            pDst[i] = pDst[i] << 1;
        }
    }
    else
    {
        /* Calculation of RFFT of input */

        /* Complex FFT process */
        arm_cfft_q15(S_CFFT, pSrc, S->ifftFlagR, S->bitReverseFlagR);

        /*  Real FFT core process */
        arm_split_rfft_q15(pSrc, L2, S->pTwiddleAReal,
                            S->pTwiddleBReal, pDst, S->twidCoefRModifier);
    }
}

/* --------------------------------------------------------------------
 * arm_split_rfft_q15 - Core Real FFT process (CM0 plain C path)
 * -------------------------------------------------------------------- */
void arm_split_rfft_q15(
    q15_t * pSrc,
    uint32_t fftLen,
    q15_t * pATable,
    q15_t * pBTable,
    q15_t * pDst,
    uint32_t modifier)
{
    uint32_t i;                                    /* Loop Counter */
    q31_t outR, outI;                              /* Temporary variables for output */
    q15_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
    q15_t *pSrc1, *pSrc2;

    pCoefA = &pATable[modifier * 2u];
    pCoefB = &pBTable[modifier * 2u];

    pSrc1 = &pSrc[2];
    pSrc2 = &pSrc[(2u * fftLen) - 2u];

    /* Run the below code for Cortex-M0 */
    i = 1u;

    while(i < fftLen)
    {
        /*
        outR = (pSrc[2 * i] * pATable[2 * i] - pSrc[2 * i + 1] * pATable[2 * i + 1]
        + pSrc[2 * n - 2 * i] * pBTable[2 * i] +
        pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);
        */

        outR = *pSrc1 * *pCoefA;
        outR = outR - (*(pSrc1 + 1) * *(pCoefA + 1));
        outR = outR + (*pSrc2 * *pCoefB);
        outR = (outR + (*(pSrc2 + 1) * *(pCoefB + 1))) >> 16;

        /* outI = (pIn[2 * i + 1] * pATable[2 * i] + pIn[2 * i] * pATable[2 * i + 1] +
        pIn[2 * n - 2 * i] * pBTable[2 * i + 1] -
        pIn[2 * n - 2 * i + 1] * pBTable[2 * i]);
        */

        outI = *pSrc2 * *(pCoefB + 1);
        outI = outI - (*(pSrc2 + 1) * *pCoefB);
        outI = outI + (*(pSrc1 + 1) * *pCoefA);
        outI = outI + (*pSrc1 * *(pCoefA + 1));

        /* update input pointers */
        pSrc1 += 2u;
        pSrc2 -= 2u;

        /* write output */
        pDst[2u * i] = (q15_t) outR;
        pDst[(2u * i) + 1u] = outI >> 16u;

        /* write complex conjugate output */
        pDst[(4u * fftLen) - (2u * i)] = (q15_t) outR;
        pDst[((4u * fftLen) - (2u * i)) + 1u] = -(outI >> 16u);

        /* update coefficient pointer */
        pCoefB = pCoefB + (2u * modifier);
        pCoefA = pCoefA + (2u * modifier);

        i++;
    }

    pDst[2u * fftLen] = (pSrc[0] - pSrc[1]) >> 1;
    pDst[(2u * fftLen) + 1u] = 0;

    pDst[0] = (pSrc[0] + pSrc[1]) >> 1;
    pDst[1] = 0;
}

/* --------------------------------------------------------------------
 * arm_split_rifft_q15 - Core Real IFFT process (CM0 plain C path)
 * -------------------------------------------------------------------- */
void arm_split_rifft_q15(
    q15_t * pSrc,
    uint32_t fftLen,
    q15_t * pATable,
    q15_t * pBTable,
    q15_t * pDst,
    uint32_t modifier)
{
    uint32_t i;                                    /* Loop Counter */
    q31_t outR, outI;                              /* Temporary variables for output */
    q15_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
    q15_t *pSrc1, *pSrc2;
    q15_t *pDst1 = &pDst[0];

    pCoefA = &pATable[0];
    pCoefB = &pBTable[0];

    pSrc1 = &pSrc[0];
    pSrc2 = &pSrc[2u * fftLen];

    /* Run the below code for Cortex-M0 */
    i = fftLen;

    while(i > 0u)
    {
        /*
        outR = (pIn[2 * i] * pATable[2 * i] + pIn[2 * i + 1] * pATable[2 * i + 1] +
        pIn[2 * n - 2 * i] * pBTable[2 * i] -
        pIn[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);
        */

        outR = *pSrc2 * *pCoefB;
        outR = outR - (*(pSrc2 + 1) * *(pCoefB + 1));
        outR = outR + (*pSrc1 * *pCoefA);
        outR = (outR + (*(pSrc1 + 1) * *(pCoefA + 1))) >> 16;

        /*
        outI = (pIn[2 * i + 1] * pATable[2 * i] - pIn[2 * i] * pATable[2 * i + 1] -
        pIn[2 * n - 2 * i] * pBTable[2 * i + 1] -
        pIn[2 * n - 2 * i + 1] * pBTable[2 * i]);
        */

        outI = *(pSrc1 + 1) * *pCoefA;
        outI = outI - (*pSrc1 * *(pCoefA + 1));
        outI = outI - (*pSrc2 * *(pCoefB + 1));
        outI = outI - (*(pSrc2 + 1) * *(pCoefB));

        /* update input pointers */
        pSrc1 += 2u;
        pSrc2 -= 2u;

        /* write output */
        *pDst1++ = (q15_t) outR;
        *pDst1++ = (q15_t) (outI >> 16);

        /* update coefficient pointer */
        pCoefB = pCoefB + (2u * modifier);
        pCoefA = pCoefA + (2u * modifier);

        i--;
    }
}