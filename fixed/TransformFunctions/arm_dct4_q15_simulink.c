/* ----------------------------------------------------------------------
 * arm_dct4_q15_simulink.c
 * Self-contained Q15 DCT4 for Simulink.
 * Uses CM0-only (plain C) code path from CMSIS DSP Library.
 * All helper functions inlined. arm_rfft_q15 is declared extern.
 *
 * Based on CMSIS DSP Library arm_dct4_q15.c (CM0 path only).
 * -------------------------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
 * Type definitions
 * -------------------------------------------------------------------- */
#ifndef Q15_T_DEFINED
typedef int16_t q15_t;
#define Q15_T_DEFINED
#endif
#ifndef Q31_T_DEFINED
typedef int32_t q31_t;
#define Q31_T_DEFINED
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
    q15_t normalize;
    q15_t *pTwiddle;
    q15_t *pCosFactor;
    void *pRfft;
} arm_dct4_instance_q15;

/* --------------------------------------------------------------------
 * Inlined helper functions
 * -------------------------------------------------------------------- */

static void arm_mult_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0; i < blockSize; i++)
        pDst[i] = (q15_t)(((q31_t)pSrcA[i] * pSrcB[i]) >> 15);
}

static void arm_shift_q15(q15_t *pSrc, int8_t shiftBits, q15_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0; i < blockSize; i++)
    {
        q31_t val = (q31_t)pSrc[i] << shiftBits;
        if (val > 32767)
            pDst[i] = 32767;
        else if (val < -32768)
            pDst[i] = -32768;
        else
            pDst[i] = (q15_t)val;
    }
}

static void arm_cmplx_mult_cmplx_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t numSamples)
{
    uint32_t i;
    for (i = 0; i < numSamples; i++)
    {
        q31_t aR = pSrcA[2 * i];
        q31_t aI = pSrcA[2 * i + 1];
        q31_t bR = pSrcB[2 * i];
        q31_t bI = pSrcB[2 * i + 1];
        pDst[2 * i]     = (q15_t)(((aR * bR - aI * bI) >> 15));
        pDst[2 * i + 1] = (q15_t)(((aI * bR + aR * bI) >> 15));
    }
}

/* --------------------------------------------------------------------
 * RFFT instance type (needed for extern declaration below)
 * -------------------------------------------------------------------- */
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
 * External RFFT function (provided separately)
 * -------------------------------------------------------------------- */
extern void arm_rfft_q15(const arm_rfft_instance_q15 *S, q15_t *pSrc, q15_t *pDst);

/* --------------------------------------------------------------------
 * arm_dct4_q15 - CM0-only code path
 * -------------------------------------------------------------------- */
void arm_dct4_q15(
    const arm_dct4_instance_q15 *S,
    q15_t *pState,
    q15_t *pInlineBuffer)
{
    uint32_t i;                                    /* Loop counter */
    q15_t *weights = S->pTwiddle;                  /* Pointer to the Weights table */
    q15_t *cosFact = S->pCosFactor;                /* Pointer to the cos factors table */
    q15_t *pS1, *pS2, *pbuff;                      /* Temporary pointers for input buffer and pState buffer */
    q15_t in;                                      /* Temporary variable */

    /*-------- Pre-processing ------------*/
    /* Multiplying input with cos factor i.e. r(n) = 2 * x(n) * cos(pi*(2*n+1)/(4*n)) */
    arm_mult_q15(pInlineBuffer, cosFact, pInlineBuffer, S->N);
    arm_shift_q15(pInlineBuffer, 1, pInlineBuffer, S->N);

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
    i = (uint32_t)S->Nby2;

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
    i = (uint32_t)S->N;

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
    arm_rfft_q15((const arm_rfft_instance_q15 *)S->pRfft, pInlineBuffer, pState);

    /*----------------------------------------------------------------------
     *  Step3: Multiply the FFT output with the weights.
     *----------------------------------------------------------------------*/
    arm_cmplx_mult_cmplx_q15(pState, weights, pState, S->N);

    /* The output of complex multiplication is in 3.13 format.
     * Hence changing the format of N (i.e. 2*N elements) complex numbers to 1.15 format by shifting left by 2 bits. */
    arm_shift_q15(pState, 2, pState, S->N * 2);

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
    i = ((uint32_t)S->N - 1u);

    do
    {
        /* Calculating Y4(1) to Y4(N-1) from Y2 using equation Y4(k) = Y2(k) - Y4(k-1) */
        /* pState pointer (pS1) is incremented twice as the real values are located alternatively in the array */
        in = *pS1++ - in;
        *pbuff++ = in;
        /* points to the next real value */
        pS1++;

        /* Decrement the loop counter */
        i--;
    } while (i > 0u);

    /*------------ Normalizing the output by multiplying with the normalizing factor ----------*/

    /* Initializing the loop counter */
    i = (uint32_t)S->N;

    /* pbuff initialized to the pInlineBuffer(now contains the output values) */
    pbuff = pInlineBuffer;

    do
    {
        /* Multiplying pInlineBuffer with the normalizing factor sqrt(2/N) */
        in = *pbuff;
        *pbuff++ = ((q15_t)(((q31_t)in * S->normalize) >> 15));

        /* Decrement the loop counter */
        i--;
    } while (i > 0u);
}