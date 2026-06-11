/* ----------------------------------------------------------------------
 * arm_bitreversal_mex.c
 * MEX wrapper for arm_bitreversal_f32.
 * Usage: y = arm_bitreversal_mex(x, fftSize, bitRevFactor, bitRevTable)
 * This is a test wrapper for the bit reversal function.
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <string.h>
#include "mex.h"

#include "arm_bitreversal_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, *pDst;
    uint16_t fftSize, bitRevFactor;
    uint16_t *pBitRevTab;
    size_t dataLen;
    uint32_t i;

    if (nrhs != 4)
        mexErrMsgIdAndTxt("arm_bitreversal_mex:nrhs", "Usage: y = arm_bitreversal_mex(x, fftSize, bitRevFactor, bitRevTable)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_bitreversal_mex:nlhs", "One output required.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_bitreversal_mex:notSingle", "Input must be real single.");

    dataLen = mxGetNumberOfElements(prhs[0]);
    if (dataLen % 2 != 0)
        mexErrMsgIdAndTxt("arm_bitreversal_mex:even", "Input length must be even (complex interleaved).");

    fftSize = (uint16_t)mxGetScalar(prhs[1]);
    bitRevFactor = (uint16_t)mxGetScalar(prhs[2]);

    if (!mxIsUint16(prhs[3]))
        mexErrMsgIdAndTxt("arm_bitreversal_mex:tableType", "bitRevTable must be uint16.");

    pBitRevTab = (uint16_t *)mxGetData(prhs[3]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)dataLen, mxSINGLE_CLASS, mxREAL);
    pSrc = (float *)mxGetData(prhs[0]);
    pDst = (float *)mxGetData(plhs[0]);

    memcpy(pDst, pSrc, dataLen * sizeof(float));
    arm_bitreversal_f32(pDst, fftSize, bitRevFactor, pBitRevTab);
}