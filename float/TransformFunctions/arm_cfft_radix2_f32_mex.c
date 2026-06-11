/* ----------------------------------------------------------------------
 * arm_cfft_radix2_f32_mex.c
 * MEX wrapper for arm_cfft_radix2_f32.
 * Usage: y = arm_cfft_radix2_f32_mex(x, ifftFlag)
 *   x: single vector (interleaved real,imag), length 2*fftLen
 *   ifftFlag: 0=forward, 1=inverse
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mex.h"

#include "arm_cfft_radix2_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, *pData;
    size_t dataLen;
    uint16_t fftLen;
    uint8_t ifftFlag;
    arm_cfft_radix2_instance_f32 S;

    if (nrhs < 1 || nrhs > 2)
        mexErrMsgIdAndTxt("arm_cfft_radix2_f32_mex:nrhs", "Usage: y = arm_cfft_radix2_f32_mex(x, ifftFlag)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_cfft_radix2_f32_mex:nlhs", "One output allowed.");

    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_cfft_radix2_f32_mex:type", "Input must be real single.");

    dataLen = mxGetNumberOfElements(prhs[0]);
    if (dataLen % 2 != 0)
        mexErrMsgIdAndTxt("arm_cfft_radix2_f32_mex:even", "Input length must be even.");

    fftLen = (uint16_t)(dataLen / 2);

    ifftFlag = 0;
    if (nrhs >= 2)
        ifftFlag = (uint8_t)(mxGetScalar(prhs[1]) != 0);

    /* Set up instance */
    S.fftLen = fftLen;
    S.ifftFlag = ifftFlag;
    S.bitReverseFlag = 1;

    /* Generate tables */
    S.pTwiddle = generate_twiddle_radix2_fn(fftLen);
    S.pBitRevTable = generate_bitrev_radix2_fn(fftLen);

    switch (fftLen) {
        case 4096: S.twidCoefModifier = 1u;   S.bitRevFactor = 1u;   S.onebyfftLen = 0.000244140625f;   break;
        case 2048: S.twidCoefModifier = 2u;   S.bitRevFactor = 2u;   S.onebyfftLen = 0.00048828125f;    break;
        case 1024: S.twidCoefModifier = 4u;   S.bitRevFactor = 4u;   S.onebyfftLen = 0.0009765625f;    break;
        case 512:  S.twidCoefModifier = 8u;   S.bitRevFactor = 8u;   S.onebyfftLen = 0.001953125f;     break;
        case 256:  S.twidCoefModifier = 16u;  S.bitRevFactor = 16u;  S.onebyfftLen = 0.00390625f;      break;
        case 128:  S.twidCoefModifier = 32u;  S.bitRevFactor = 32u;  S.onebyfftLen = 0.0078125f;       break;
        case 64:   S.twidCoefModifier = 64u;  S.bitRevFactor = 64u;  S.onebyfftLen = 0.015625f;        break;
        case 32:   S.twidCoefModifier = 128u; S.bitRevFactor = 128u; S.onebyfftLen = 0.03125f;         break;
        case 16:   S.twidCoefModifier = 256u; S.bitRevFactor = 256u; S.onebyfftLen = 0.0625f;          break;
        default:
            free(S.pTwiddle); free(S.pBitRevTable);
            mexErrMsgIdAndTxt("arm_cfft_radix2_f32_mex:len", "Unsupported FFT length.");
    }

    /* Create output (copy input, process in-place) */
    plhs[0] = mxCreateNumericMatrix(1, (mwSize)dataLen, mxSINGLE_CLASS, mxREAL);
    pSrc = (float *)mxGetData(prhs[0]);
    pData = (float *)mxGetData(plhs[0]);
    memcpy(pData, pSrc, dataLen * sizeof(float));

    arm_cfft_radix2_f32(&S, pData);

    free(S.pTwiddle);
    free(S.pBitRevTable);
}