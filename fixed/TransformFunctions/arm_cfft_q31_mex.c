/* ----------------------------------------------------------------------
 * arm_cfft_q31_mex.c
 * MEX wrapper for arm_cfft_q31.
 * Includes the self-contained simulink implementation.
 *
 * Usage:
 *   y = arm_cfft_q31_mex(x, ifftFlag)
 *   x: int32 vector of length 2*fftLen (interleaved real,imag)
 *   ifftFlag: 0=forward FFT, 1=inverse IFFT
 * -------------------------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mex.h"

/* Include the self-contained implementation */
#include "arm_cfft_q31_simulink.c"

/* MEX wrapper */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrc;
    size_t dataLen;
    uint32_t fftLen;
    uint8_t ifftFlag;
    arm_cfft_instance_q31 S;
    q31_t *pData;

    /* Check inputs */
    if (nrhs < 1 || nrhs > 2)
        mexErrMsgIdAndTxt("arm_cfft_q31_mex:invalidInput", "Usage: y = arm_cfft_q31_mex(x, ifftFlag)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_cfft_q31_mex:invalidOutput", "At most 1 output allowed.");

    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_cfft_q31_mex:invalidInputType", "Input must be real int32.");

    dataLen = mxGetNumberOfElements(prhs[0]);
    if (dataLen % 2 != 0)
        mexErrMsgIdAndTxt("arm_cfft_q31_mex:invalidDim", "Input length must be even (complex interleaved).");

    fftLen = (uint32_t)(dataLen / 2);

    /* Validate FFT size */
    if (fftLen != 16 && fftLen != 32 && fftLen != 64 && fftLen != 128 &&
        fftLen != 256 && fftLen != 512 && fftLen != 1024 && fftLen != 2048 && fftLen != 4096)
        mexErrMsgIdAndTxt("arm_cfft_q31_mex:invalidFFTSize",
            "FFT length must be one of: 16, 32, 64, 128, 256, 512, 1024, 2048, 4096.");

    ifftFlag = 0;
    if (nrhs >= 2) {
        if (!mxIsDouble(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
            mexErrMsgIdAndTxt("arm_cfft_q31_mex:invalidFlag", "ifftFlag must be a scalar double.");
        ifftFlag = (uint8_t)(mxGetScalar(prhs[1]) != 0);
    }

    /* Set up instance struct */
    S.fftLen = fftLen;

    /* Generate twiddle table for the requested FFT size */
    {
        uint32_t twidLen = (3u * fftLen) / 4u;
        uint32_t i;
        double pi = 3.14159265358979323846;

        q31_t *twiddle = (q31_t *)mxCalloc(twidLen * 2, sizeof(q31_t));
        if (!twiddle) mexErrMsgIdAndTxt("arm_cfft_q31_mex:alloc", "Failed to allocate twiddle table.");

        for (i = 0; i < twidLen; i++) {
            double angle = 2.0 * pi * (double)i / (double)fftLen;
            double c = cos(angle);
            double s = sin(angle);
            twiddle[2u * i]     = (q31_t)((int64_t)(c * 2147483647.0 + (c >= 0 ? 0.5 : -0.5)));
            twiddle[2u * i + 1u] = (q31_t)((int64_t)(s * 2147483647.0 + (s >= 0 ? 0.5 : -0.5)));
        }
        S.pTwiddle = twiddle;
    }

    /* Use pre-computed CMSIS bit-reversal tables from the simulink file */
    S.bitRevLength = 0;
    S.pBitRevTable = NULL;
    switch (fftLen) {
        case 16:
            S.pBitRevTable = (uint16_t *)armBitRevIndexTable_fixed_16_q31;
            S.bitRevLength = ARMBITREVINDEXTABLE_FIXED___16_TABLE_LENGTH;
            break;
        case 32:
            S.pBitRevTable = (uint16_t *)armBitRevIndexTable_fixed_32_q31;
            S.bitRevLength = ARMBITREVINDEXTABLE_FIXED___32_TABLE_LENGTH;
            break;
        case 64:
            S.pBitRevTable = (uint16_t *)armBitRevIndexTable_fixed_64_q31;
            S.bitRevLength = ARMBITREVINDEXTABLE_FIXED___64_TABLE_LENGTH;
            break;
        case 128:
            S.pBitRevTable = (uint16_t *)armBitRevIndexTable_fixed_128_q31;
            S.bitRevLength = ARMBITREVINDEXTABLE_FIXED__128_TABLE_LENGTH;
            break;
        case 256:
            S.pBitRevTable = (uint16_t *)armBitRevIndexTable_fixed_256_q31;
            S.bitRevLength = ARMBITREVINDEXTABLE_FIXED__256_TABLE_LENGTH;
            break;
        default:
            mexErrMsgIdAndTxt("arm_cfft_q31_mex:noTable",
                "Pre-computed bit-reversal table not available for N=%d.", fftLen);
            break;
    }

    /* Create output */
    plhs[0] = mxCreateNumericMatrix(1, (mwSize)dataLen, mxINT32_CLASS, mxREAL);
    pSrc = (int32_t *)mxGetData(prhs[0]);
    pData = (q31_t *)mxGetData(plhs[0]);

    /* Copy input to output (in-place processing) */
    memcpy(pData, pSrc, dataLen * sizeof(int32_t));

    /* Run the FFT */
    arm_cfft_q31(&S, pData, ifftFlag, 1);

    /* Free allocated twiddle table */
    mxFree(S.pTwiddle);
}