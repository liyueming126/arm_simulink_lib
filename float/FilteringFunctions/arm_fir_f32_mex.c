/* MEX wrapper for arm_fir_f32 */
#include "mex.h"
#include <string.h>

/* Include the simplified implementation */
#include "arm_fir_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_fir_instance_f32 S;
    float32_t *pSrc, *pDst;
    uint32_t blockSize;
    uint16_t numTaps;
    float32_t *pCoeffs;
    float32_t *pState;
    uint32_t stateSize;
    mwSize i;

    /* Check inputs: coefficients, input signal */
    if (nrhs != 2) {
        mexErrMsgIdAndTxt("arm_fir_f32:InvalidInput", "Two inputs required: coefficients, input signal");
    }
    if (nlhs > 1) {
        mexErrMsgIdAndTxt("arm_fir_f32:InvalidOutput", "One output required");
    }
    if (!mxIsSingle(prhs[0]) || !mxIsSingle(prhs[1])) {
        mexErrMsgIdAndTxt("arm_fir_f32:NotSingle", "Inputs must be single precision");
    }

    pCoeffs = (float32_t *)mxGetData(prhs[0]);
    pSrc = (float32_t *)mxGetData(prhs[1]);
    numTaps = (uint16_t)mxGetNumberOfElements(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[1]);

    stateSize = numTaps + blockSize - 1;
    pState = (float32_t *)mxCalloc(stateSize, sizeof(float32_t));

    /* Init instance */
    S.numTaps = numTaps;
    S.pCoeffs = pCoeffs;
    S.pState = pState;
    for (i = 0; i < stateSize; i++) pState[i] = 0.0f;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    pDst = (float32_t *)mxGetData(plhs[0]);

    arm_fir_f32(&S, pSrc, pDst, blockSize);

    mxFree(pState);
}