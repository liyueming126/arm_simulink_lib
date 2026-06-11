/* MEX wrapper for arm_fir_sparse_f32 */
#include "mex.h"
#include <string.h>

#include "arm_fir_sparse_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_fir_sparse_instance_f32 S;
    float32_t *pSrc, *pDst, *pCoeffs, *pState, *pScratchIn;
    int32_t *pTapDelay;
    uint16_t numTaps, maxDelay;
    uint32_t blockSize, delaySize;
    mwSize i;

    if (nrhs != 4) {
        mexErrMsgIdAndTxt("arm_fir_sparse_f32:InvalidInput", "Four inputs: coefficients, tapDelays, maxDelay, input signal");
    }
    if (nlhs > 1) {
        mexErrMsgIdAndTxt("arm_fir_sparse_f32:InvalidOutput", "One output required");
    }
    if (!mxIsSingle(prhs[0]) || !mxIsSingle(prhs[3])) {
        mexErrMsgIdAndTxt("arm_fir_sparse_f32:NotSingle", "Coefficients and input must be single precision");
    }

    pCoeffs = (float32_t *)mxGetData(prhs[0]);
    pTapDelay = (int32_t *)mxGetData(prhs[1]);
    if (!mxIsInt32(prhs[1])) {
        mexErrMsgIdAndTxt("arm_fir_sparse_f32:NotInt32", "TapDelays must be int32");
    }

    maxDelay = (uint16_t)mxGetScalar(prhs[2]);
    pSrc = (float32_t *)mxGetData(prhs[3]);
    numTaps = (uint16_t)mxGetNumberOfElements(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[3]);

    delaySize = maxDelay + blockSize;
    pState = (float32_t *)mxCalloc(delaySize, sizeof(float32_t));
    pScratchIn = (float32_t *)mxCalloc(blockSize, sizeof(float32_t));

    S.numTaps = numTaps;
    S.stateIndex = 0;
    S.pState = pState;
    S.pCoeffs = pCoeffs;
    S.maxDelay = maxDelay;
    S.pTapDelay = pTapDelay;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    pDst = (float32_t *)mxGetData(plhs[0]);

    arm_fir_sparse_f32(&S, pSrc, pDst, pScratchIn, blockSize);

    mxFree(pState);
    mxFree(pScratchIn);
}