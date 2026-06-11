/* ----------------------------------------------------------------------
 * arm_cfft_f32_mex.c
 * MEX wrapper for arm_cfft_f32 (modern mixed-radix CFFT).
 * Usage: y = arm_cfft_f32_mex(x, ifftFlag)
 *   x: single vector (interleaved real,imag), length 2*fftLen
 *   ifftFlag: 0=forward FFT, 1=inverse IFFT
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mex.h"

#include "arm_cfft_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, *pData;
    size_t dataLen;
    uint16_t fftLen;
    uint8_t ifftFlag;
    arm_cfft_instance_f32 S;

    if (nrhs < 1 || nrhs > 2)
        mexErrMsgIdAndTxt("arm_cfft_f32_mex:nrhs", "Usage: y = arm_cfft_f32_mex(x, ifftFlag)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_cfft_f32_mex:nlhs", "One output allowed.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_cfft_f32_mex:type", "Input must be real single.");

    dataLen = mxGetNumberOfElements(prhs[0]);
    if (dataLen % 2 != 0)
        mexErrMsgIdAndTxt("arm_cfft_f32_mex:even", "Input length must be even.");
    fftLen = (uint16_t)(dataLen / 2);

    /* Validate supported lengths */
    if (fftLen != 16 && fftLen != 32 && fftLen != 64 && fftLen != 128 &&
        fftLen != 256 && fftLen != 512 && fftLen != 1024 && fftLen != 2048 && fftLen != 4096)
        mexErrMsgIdAndTxt("arm_cfft_f32_mex:len",
            "FFT length must be one of: 16, 32, 64, 128, 256, 512, 1024, 2048, 4096.");

    ifftFlag = (nrhs >= 2) ? (uint8_t)(mxGetScalar(prhs[1]) != 0) : 0;

    /* Set up instance structure */
    S.fftLen = fftLen;

    /* Generate twiddle table */
    float *twiddle = generate_twiddle_cfft(fftLen);
    S.pTwiddle = twiddle;

    /* Generate bit reversal table */
    uint16_t *bitRevTbl = generate_bitrev_table_cfft(fftLen);
    S.pBitRevTable = bitRevTbl;
    S.bitRevLength = 2u * fftLen; /* enough for all swap pairs */

    /* Create output (copy input, process in-place) */
    plhs[0] = mxCreateNumericMatrix(1, (mwSize)dataLen, mxSINGLE_CLASS, mxREAL);
    pSrc = (float *)mxGetData(prhs[0]);
    pData = (float *)mxGetData(plhs[0]);
    memcpy(pData, pSrc, dataLen * sizeof(float));

    arm_cfft_f32(&S, pData, ifftFlag, 1);

    free((void *)twiddle);
    free((void *)bitRevTbl);
}