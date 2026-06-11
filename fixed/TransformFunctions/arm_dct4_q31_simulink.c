/* ----------------------------------------------------------------------
 * arm_dct4_q31_simulink.c
 * Self-contained Q31 DCT4 for Simulink.
 * Uses CM0-only (plain C) code path from CMSIS DSP Library.
 * All helper functions inlined. arm_rfft_q31 is declared extern.
 *
 * Based on CMSIS DSP Library arm_dct4_q31.c (CM0 path only).
 * -------------------------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
 * Type definitions
 * -------------------------------------------------------------------- */
#ifndef Q31_T_DEFINED
typedef int32_t q31_t;
#define Q31_T_DEFINED
#endif
#ifndef Q63_T_DEFINED
typedef int64_t q63_t;
#define Q63_T_DEFINED
#endif

typedef enum { ARM_MATH_SUCCESS = 0, ARM_MATH_ARGUMENT_ERROR = 1 } arm_status;

/* --------------------------------------------------------------------
 * CMSIS-compatible struct types
 * -------------------------------------------------------------------- */

/* DCT4 instance */
typedef struct
{
    uint16_t N;
    uint16_t Nby2;
    q31_t normalize;
    q31_t *pTwiddle;
    q31_t *pCosFactor;
    void *pRfft;
} arm_dct4_instance_q31;

/* --------------------------------------------------------------------
 * Inlined helper functions
 * -------------------------------------------------------------------- */

static void arm_mult_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0; i < blockSize; i++)
        pDst[i] = (q31_t)(((q63_t)pSrcA[i] * pSrcB[i]) >> 31);
}

static void arm_shift_q31(q31_t *pSrc, int8_t shiftBits, q31_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0; i < blockSize; i++)
    {
        q63_t val = (q63_t)pSrc[i] << shiftBits;
        if (val > 2147483647LL)
            pDst[i] = 2147483647;
        else if (val < -2147483648LL)
            pDst[i] = -2147483648;
        else
            pDst[i] = (q31_t)val;
    }
}

static void arm_cmplx_mult_cmplx_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t numSamples)
{
    uint32_t i;
    for (i = 0; i < numSamples; i++)
    {
        q63_t aR = pSrcA[2 * i];
        q63_t aI = pSrcA[2 * i + 1];
        q63_t bR = pSrcB[2 * i];
        q63_t bI = pSrcB[2 * i + 1];
        pDst[2 * i]     = (q31_t)((aR * bR - aI * bI) >> 31);
        pDst[2 * i + 1] = (q31_t)((aI * bR + aR * bI) >> 31);
    }
}

/* --------------------------------------------------------------------
 * RFFT instance type (needed for extern declaration below)
 * -------------------------------------------------------------------- */
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
 * External RFFT function (provided separately)
 * -------------------------------------------------------------------- */
extern void arm_rfft_q31(const arm_rfft_instance_q31 *S, q31_t *pSrc, q31_t *pDst);

/* --------------------------------------------------------------------
 * arm_dct4_q31 - CM0-only code path
 * -------------------------------------------------------------------- */
void arm_dct4_q31(
    const arm_dct4_instance_q31 *S,
    q31_t *pState,
    q31_t *pInlineBuffer)
{
    uint16_t i;                                    /* Loop counter */
    q31_t *weights = S->pTwiddle;                  /* Pointer to the Weights table */
    q31_t *cosFact = S->pCosFactor;                /* Pointer to the cos factors table */
    q31_t *pS1, *pS2, *pbuff;                      /* Temporary pointers for input buffer and pState buffer */
    q31_t in;                                      /* Temporary variable */

    /*-------- Pre-processing ------------*/
    /* Multiplying input with cos factor i.e. r(n) = 2 * x(n) * cos(pi*(2*n+1)/(4*n)) */
    arm_mult_q31(pInlineBuffer, cosFact, pInlineBuffer, S->N);
    arm_shift_q31(pInlineBuffer, 1, pInlineBuffer, S->N);

    /* ----------------------------------------------------------------
     * Step1: Re-ordering of even and odd elements as
     *             pState[i] =  pInlineBuffer[2*i] and
     *             pState[N-i-1] = pInlineBuffer[2*i+1] where i = 0 to N/2
     * ---------------------------------------------------------------- */

    /* pS1 initialized to pState */
    pS1 = pState;

    /* pS2 initialized to pState+N-1, so that it points to the end of the state buffer */
    pS2 = pState + (S->N - 1u);

    /* pbuff initialized to input buffer */
    pbuff = pInlineBuffer;

    /* Run the below code for Cortex-M0 */

    /* Initializing the loop counter to N/2 */
    i = S->Nby2;

    do
    {
        /* Re-ordering of even and odd elements */
        /* pState[i] =  pInlineBuffer[2*i] */
        *pS1++ = *pbuff++;
        /* pState[N-i-1] = pInlineBuffer[2*i+1] */
        *pS2-- = *pbuff++;

        /* Decrement the loop counter */
        i--;
    } while (i > 0u);

    /* pbuff initialized to input buffer */
    pbuff = pInlineBuffer;

    /* pS1 initialized to pState */
    pS1 = pState;

    /* Initializing the loop counter */
    i = S->N;

    do
    {
        /* Writing the re-ordered output back to inplace input buffer */
        *pbuff++ = *pS1++;

        /* Decrement the loop counter */
        i--;
    } while (i > 0u);

    /* ---------------------------------------------------------
     *     Step2: Calculate RFFT for N-point input
     * ---------------------------------------------------------- */
    /* pInlineBuffer is real input of length N , pState is the complex output of length 2N */
    arm_rfft_q31((const arm_rfft_instance_q31 *)S->pRfft, pInlineBuffer, pState);

    /*----------------------------------------------------------------------
     *  Step3: Multiply the FFT output with the weights.
     *----------------------------------------------------------------------*/
    arm_cmplx_mult_cmplx_q31(pState, weights, pState, S->N);

    /* The output of complex multiplication is in 3.29 format.
     * Hence changing the format of N (i.e. 2*N elements) complex numbers to 1.31 format by shifting left by 2 bits. */
    arm_shift_q31(pState, 2, pState, S->N * 2);

    /* ----------- Post-processing ---------- */
    /* DCT-IV can be obtained from DCT-II by the equation,
     *       Y4(k) = Y2(k) - Y4(k-1) and Y4(-1) = Y4(0)
     *       Hence, Y4(0) = Y2(0)/2  */
    /* Getting only real part from the output and Converting to DCT-IV */

    /* pbuff initialized to input buffer. */
    pbuff = pInlineBuffer;

    /* pS1 initialized to pState */
    pS1 = pState;

    /* Calculating Y4(0) from Y2(0) using Y4(0) = Y2(0)/2 */
    in = *pS1++ >> 1u;
    /* input buffer acts as inplace, so output values are stored in the input itself. */
    *pbuff++ = in;

    /* pState pointer is incremented twice as the real values are located alternatively in the array */
    pS1++;

    /* Initializing the loop counter */
    i = (S->N - 1u);

    while (i > 0u)
    {
        /* Calculating Y4(1) to Y4(N-1) from Y2 using equation Y4(k) = Y2(k) - Y4(k-1) */
        /* pState pointer (pS1) is incremented twice as the real values are located alternatively in the array */
        in = *pS1++ - in;
        *pbuff++ = in;
        /* points to the next real value */
        pS1++;

        /* Decrement the loop counter */
        i--;
    }

    /*------------ Normalizing the output by multiplying with the normalizing factor ----------*/

    /* Initializing the loop counter */
    i = S->N;

    /* pbuff initialized to the pInlineBuffer(now contains the output values) */
    pbuff = pInlineBuffer;

    do
    {
        /* Multiplying pInlineBuffer with the normalizing factor sqrt(2/N) */
        in = *pbuff;
        *pbuff++ = ((q31_t)(((q63_t)in * S->normalize) >> 31));

        /* Decrement the loop counter */
        i--;
    } while (i > 0u);
}