/* MEX wrapper for arm_fir_decimate_f32 */
#include "mex.h"
#include <string.h>

#include "arm_fir_decimate_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_fir_decimate_instance_f32 S;
    float32_t *pSrc, *pDst, *pCoeffs, *pState;
    uint32_t blockSize, stateSize;
    uint16_t numTaps;
    uint8_t M;
    mwSize outSize;
    mwSize i;

    if (nrhs != 3) {
        mexErrMsgIdAndTxt("arm_fir_decimate_f32:InvalidInput", "Three inputs: decimation factor, coefficients, input signal");
    }
    if (nlhs > 1) {
        mexErrMsgIdAndTxt("arm_fir_decimate_f32:InvalidOutput", "One output required");
    }
    if (!mxIsSingle(prhs[1]) || !mxIsSingle(prhs[2])) {
        mexErrMsgIdAndTxt("arm_fir_decimate_f32:NotSingle", "Inputs must be single precision");
    }

    M = (uint8_t)mxGetScalar(prhs[0]);
    pCoeffs = (float32_t *)mxGetData(prhs[1]);
    pSrc = (float32_t *)mxGetData(prhs[2]);
    numTaps = (uint16_t)mxGetNumberOfElements(prhs[1]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[2]);

    if (blockSize % M != 0) {
        mexErrMsgIdAndTxt("arm_fir_decimate_f32:InvalidBlockSize", "blockSize must be multiple of M");
    }

    stateSize = numTaps + blockSize - 1;
    pState = (float32_t *)mxCalloc(stateSize, sizeof(float32_t));
    for (i = 0; i < (mwSize)stateSize; i++) pState[i] = 0.0f;

    S.M = M;
    S.numTaps = numTaps;
    S.pCoeffs = pCoeffs;
    S.pState = pState;

    outSize = (mwSize)(blockSize / M);
    plhs[0] = mxCreateNumericMatrix(1, outSize, mxSINGLE_CLASS, mxREAL);
    pDst = (float32_t *)mxGetData(plhs[0]);

    arm_fir_decimate_f32(&S, pSrc, pDst, blockSize);

    mxFree(pState);
}