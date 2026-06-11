/* MEX wrapper for arm_lms_norm_f32 */
#include "mex.h"
#include <string.h>

#include "arm_lms_norm_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_lms_norm_instance_f32 S;
    float32_t *pSrc, *pRef, *pOut, *pErr, *pCoeffs, *pState;
    uint32_t blockSize;
    mwSize stateSize, i;

    if (nrhs != 4) {
        mexErrMsgIdAndTxt("arm_lms_norm_f32:InvalidInput", "Four inputs: mu, coefficients, input signal, reference signal");
    }
    if (nlhs != 2) {
        mexErrMsgIdAndTxt("arm_lms_norm_f32:InvalidOutput", "Two outputs required: output signal, error signal");
    }
    if (!mxIsSingle(prhs[1]) || !mxIsSingle(prhs[2]) || !mxIsSingle(prhs[3])) {
        mexErrMsgIdAndTxt("arm_lms_norm_f32:NotSingle", "Inputs must be single precision");
    }

    S.mu = (float32_t)mxGetScalar(prhs[0]);
    pCoeffs = (float32_t *)mxGetData(prhs[1]);
    S.pCoeffs = pCoeffs;
    pSrc = (float32_t *)mxGetData(prhs[2]);
    pRef = (float32_t *)mxGetData(prhs[3]);
    S.numTaps = (uint16_t)mxGetNumberOfElements(prhs[1]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[2]);

    S.energy = 0.0f;
    S.x0 = 0.0f;

    stateSize = S.numTaps + blockSize - 1;
    pState = (float32_t *)mxCalloc(stateSize, sizeof(float32_t));
    for (i = 0; i < stateSize; i++) pState[i] = 0.0f;
    S.pState = pState;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    plhs[1] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    pOut = (float32_t *)mxGetData(plhs[0]);
    pErr = (float32_t *)mxGetData(plhs[1]);

    arm_lms_norm_f32(&S, pSrc, pRef, pOut, pErr, blockSize);

    mxFree(pState);
}