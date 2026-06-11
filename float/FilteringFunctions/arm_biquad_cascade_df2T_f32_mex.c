/* MEX wrapper for arm_biquad_cascade_df2T_f32 */
#include "mex.h"
#include <string.h>

#include "arm_biquad_cascade_df2T_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_biquad_cascade_df2T_instance_f32 S;
    float32_t *pSrc, *pDst, *pCoeffs, *pState;
    uint32_t blockSize;
    mwSize i;

    if (nrhs != 3) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df2T_f32:InvalidInput", "Three inputs: numStages, coefficients [b0,b1,b2,a1,a2 per stage], input signal");
    }
    if (nlhs > 1) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df2T_f32:InvalidOutput", "One output required");
    }
    if (!mxIsSingle(prhs[1]) || !mxIsSingle(prhs[2])) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df2T_f32:NotSingle", "Inputs must be single precision");
    }

    S.numStages = (uint8_t)mxGetScalar(prhs[0]);
    pCoeffs = (float32_t *)mxGetData(prhs[1]);
    S.pCoeffs = pCoeffs;
    pSrc = (float32_t *)mxGetData(prhs[2]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[2]);

    /* 2 state variables per stage: d1, d2 */
    pState = (float32_t *)mxCalloc(2 * S.numStages, sizeof(float32_t));
    for (i = 0; i < 2 * S.numStages; i++) pState[i] = 0.0f;
    S.pState = pState;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    pDst = (float32_t *)mxGetData(plhs[0]);

    arm_biquad_cascade_df2T_f32(&S, pSrc, pDst, blockSize);

    mxFree(pState);
}