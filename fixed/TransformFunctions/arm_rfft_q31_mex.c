/* ----------------------------------------------------------------------
 * arm_rfft_q31_mex.c
 * MEX wrapper for arm_rfft_q31.
 * Includes arm_cfft_q31_simulink.c and arm_rfft_q31_simulink.c.
 *
 * Usage:
 *   y = arm_rfft_q31_mex(x, ifftFlag)
 *   Forward RFFT (ifftFlag=0):
 *     x: int32 vector of length N (real samples)
 *     y: int32 vector of length 2*N (interleaved complex, N = fftLenReal)
 *   Inverse RIFFT (ifftFlag=1):
 *     x: int32 vector of length 2*N (interleaved complex)
 *     y: int32 vector of length N (real samples)
 * -------------------------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mex.h"

/* Include the self-contained implementations */
#include "arm_cfft_q31_simulink.c"
#include "arm_rfft_q31_simulink.c"

/* --------------------------------------------------------------------
 * Generate RFFT twiddle tables A and B (Q31)
 * Based on CMSIS formulas with n_table = 4096 max.
 * -------------------------------------------------------------------- */
static void generate_real_twiddle_q31(q31_t *pA, q31_t *pB, uint32_t nPairs)
{
    uint32_t i;
    double pi = 3.14159265358979323846;
    double step = pi / 4096.0;  /* PI / max_n, where max_n = 4096 */

    for (i = 0; i < nPairs; i++) {
        double angle = step * (double)i;
        double sin_a = sin(angle);
        double cos_a = cos(angle);
        double a0 = 0.5 * (1.0 - sin_a);
        double a1 = 0.5 * (-cos_a);
        double b0 = 0.5 * (1.0 + sin_a);
        double b1 = 0.5 * cos_a;

        pA[2u * i]     = (q31_t)((int64_t)(a0 * 2147483647.0 + (a0 >= 0.0 ? 0.5 : -0.5)));
        pA[2u * i + 1u] = (q31_t)((int64_t)(a1 * 2147483647.0 + (a1 >= 0.0 ? 0.5 : -0.5)));
        pB[2u * i]     = (q31_t)((int64_t)(b0 * 2147483647.0 + (b0 >= 0.0 ? 0.5 : -0.5)));
        pB[2u * i + 1u] = (q31_t)((int64_t)(b1 * 2147483647.0 + (b1 >= 0.0 ? 0.5 : -0.5)));
    }
}

/* --------------------------------------------------------------------
 * MEX entry point
 * -------------------------------------------------------------------- */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrc;
    size_t dataLen;
    uint32_t fftLenReal;
    uint8_t ifftFlag;
    arm_rfft_instance_q31 S_RFFT;
    arm_cfft_instance_q31 *S_CFFT;
    q31_t *pDataIn, *pDataOut;

    /* Check inputs */
    if (nrhs < 1 || nrhs > 2)
        mexErrMsgIdAndTxt("arm_rfft_q31_mex:invalidInput",
            "Usage: y = arm_rfft_q31_mex(x, ifftFlag)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_rfft_q31_mex:invalidOutput",
            "At most 1 output allowed.");

    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_rfft_q31_mex:invalidInputType",
            "Input must be real int32.");

    dataLen = mxGetNumberOfElements(prhs[0]);

    ifftFlag = 0;
    if (nrhs >= 2) {
        if (!mxIsDouble(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
            mexErrMsgIdAndTxt("arm_rfft_q31_mex:invalidFlag",
                "ifftFlag must be a scalar double.");
        ifftFlag = (uint8_t)(mxGetScalar(prhs[1]) != 0);
    }

    if (ifftFlag == 0) {
        /* Forward RFFT: input is N real values, output is N complex (2N values) */
        if (dataLen % 2 != 0)
            mexErrMsgIdAndTxt("arm_rfft_q31_mex:invalidDim",
                "RFFT input length must be even (N real samples).");
        if (dataLen < 32)
            mexErrMsgIdAndTxt("arm_rfft_q31_mex:invalidDim",
                "RFFT input length must be at least 32.");

        fftLenReal = (uint32_t)dataLen;
    } else {
        /* Inverse RIFFT: input is N complex values (2N elements), output is N real */
        if (dataLen % 4 != 0)
            mexErrMsgIdAndTxt("arm_rfft_q31_mex:invalidDim",
                "RIFFT input length must be a multiple of 4 (complex interleaved).");
        if (dataLen < 64)
            mexErrMsgIdAndTxt("arm_rfft_q31_mex:invalidDim",
                "RIFFT input length must be at least 64.");

        fftLenReal = (uint32_t)(dataLen / 2);
    }

    /* Validate supported FFT sizes */
    if (fftLenReal != 32 && fftLenReal != 64 && fftLenReal != 128 &&
        fftLenReal != 256 && fftLenReal != 512 && fftLenReal != 1024 &&
        fftLenReal != 2048 && fftLenReal != 4096 && fftLenReal != 8192)
        mexErrMsgIdAndTxt("arm_rfft_q31_mex:invalidFFTSize",
            "Real FFT length must be one of: 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192.");

    /* Set up RFFT instance */
    S_RFFT.fftLenReal = fftLenReal;
    S_RFFT.fftLenBy2 = fftLenReal >> 1;
    S_RFFT.ifftFlagR = ifftFlag;
    S_RFFT.bitReverseFlagR = 1;

    /* Set modifier based on FFT size (matches CMSIS init) */
    switch (fftLenReal) {
    case 8192u: S_RFFT.twidCoefRModifier = 1u;   break;
    case 4096u: S_RFFT.twidCoefRModifier = 2u;   break;
    case 2048u: S_RFFT.twidCoefRModifier = 4u;   break;
    case 1024u: S_RFFT.twidCoefRModifier = 8u;   break;
    case 512u:  S_RFFT.twidCoefRModifier = 16u;  break;
    case 256u:  S_RFFT.twidCoefRModifier = 32u;  break;
    case 128u:  S_RFFT.twidCoefRModifier = 64u;  break;
    case 64u:   S_RFFT.twidCoefRModifier = 128u; break;
    case 32u:   S_RFFT.twidCoefRModifier = 256u; break;
    default:
        mexErrMsgIdAndTxt("arm_rfft_q31_mex:invalidFFTSize",
            "Unsupported real FFT length.");
    }

    /* Generate A and B twiddle tables (4096 complex pairs = 8192 entries) */
    {
        uint32_t tblLen = 4096u;  /* max complex pairs */
        q31_t *pA, *pB;

        pA = (q31_t *)mxCalloc(tblLen * 2u, sizeof(q31_t));
        pB = (q31_t *)mxCalloc(tblLen * 2u, sizeof(q31_t));
        if (!pA || !pB)
            mexErrMsgIdAndTxt("arm_rfft_q31_mex:alloc",
                "Failed to allocate RFFT twiddle tables.");

        generate_real_twiddle_q31(pA, pB, tblLen);
        S_RFFT.pTwiddleAReal = pA;
        S_RFFT.pTwiddleBReal = pB;
    }

    /* Set up CFFT instance for the inner complex FFT */
    {
        uint32_t cfftLen = S_RFFT.fftLenBy2;
        uint32_t twidLen = (3u * cfftLen) / 4u;
        uint32_t i;
        double pi = 3.14159265358979323846;

        S_CFFT = (arm_cfft_instance_q31 *)mxCalloc(1, sizeof(arm_cfft_instance_q31));
        if (!S_CFFT)
            mexErrMsgIdAndTxt("arm_rfft_q31_mex:alloc",
                "Failed to allocate CFFT instance.");

        S_CFFT->fftLen = cfftLen;

        /* Generate CFFT twiddle table */
        {
            q31_t *twiddle = (q31_t *)mxCalloc(twidLen * 2, sizeof(q31_t));
            if (!twiddle)
                mexErrMsgIdAndTxt("arm_rfft_q31_mex:alloc",
                    "Failed to allocate CFFT twiddle table.");

            for (i = 0; i < twidLen; i++) {
                double angle = 2.0 * pi * (double)i / (double)cfftLen;
                double c = cos(angle);
                double s = sin(angle);
                twiddle[2u * i]     = (q31_t)((int64_t)(c * 2147483647.0 + (c >= 0.0 ? 0.5 : -0.5)));
                twiddle[2u * i + 1u] = (q31_t)((int64_t)(s * 2147483647.0 + (s >= 0.0 ? 0.5 : -0.5)));
            }
            S_CFFT->pTwiddle = twiddle;
        }

        /* Generate bit-reversal table */
        {
            uint32_t log2N = 0, tmp = cfftLen / 4, j;
            uint16_t *bitRev;

            while (tmp >>= 1) log2N++;

            S_CFFT->bitRevLength = cfftLen / 4;
            bitRev = (uint16_t *)mxCalloc(S_CFFT->bitRevLength, sizeof(uint16_t));
            if (!bitRev)
                mexErrMsgIdAndTxt("arm_rfft_q31_mex:alloc",
                    "Failed to allocate bitrev table.");

            for (j = 0; j < S_CFFT->bitRevLength; j++) {
                uint32_t rev = 0, t = j, bits = log2N;
                while (bits--) { rev <<= 1; rev |= (t & 1); t >>= 1; }
                bitRev[j] = (uint16_t)rev;
            }
            S_CFFT->pBitRevTable = bitRev;
        }

        S_RFFT.pCfft = (void *)S_CFFT;
    }

    /* Allocate input/output buffers */
    pSrc = (int32_t *)mxGetData(prhs[0]);

    if (ifftFlag == 0) {
        /* Forward RFFT: input N real -> output 2*N complex */
        plhs[0] = mxCreateNumericMatrix(1, (mwSize)(fftLenReal * 2), mxINT32_CLASS, mxREAL);
        pDataIn = (q31_t *)mxCalloc(fftLenReal, sizeof(q31_t));
        pDataOut = (q31_t *)mxGetData(plhs[0]);

        /* Copy input (real samples) */
        memcpy(pDataIn, pSrc, fftLenReal * sizeof(int32_t));

        /* Run RFFT */
        arm_rfft_q31(&S_RFFT, pDataIn, pDataOut);

        mxFree(pDataIn);
    } else {
        /* Inverse RIFFT: input 2*N complex -> output N real */
        plhs[0] = mxCreateNumericMatrix(1, (mwSize)fftLenReal, mxINT32_CLASS, mxREAL);
        pDataIn = (q31_t *)mxCalloc(fftLenReal * 2, sizeof(q31_t));
        pDataOut = (q31_t *)mxGetData(plhs[0]);

        /* Copy input (complex interleaved spectrum) */
        memcpy(pDataIn, pSrc, fftLenReal * 2 * sizeof(int32_t));

        /* Run RIFFT */
        arm_rfft_q31(&S_RFFT, pDataIn, pDataOut);

        mxFree(pDataIn);
    }

    /* Free allocated resources */
    mxFree(S_RFFT.pTwiddleAReal);
    mxFree(S_RFFT.pTwiddleBReal);
    mxFree(S_CFFT->pTwiddle);
    mxFree(S_CFFT->pBitRevTable);
    mxFree(S_CFFT);
}