/* MEX wrapper for arm_biquad_cascade_df2T_f64 */
#include "mex.h"
#include <string.h>

#include "arm_biquad_cascade_df2T_f64_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_biquad_cascade_df2T_instance_f64 S;
    float64_t *pSrc, *pDst, *pCoeffs, *pState;
    uint32_t blockSize;
    mwSize i;

    if (nrhs != 3) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df2T_f64:InvalidInput", "Three inputs: numStages, coefficients, input signal");
    }
    if (nlhs > 1) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df2T_f64:InvalidOutput", "One output required");
    }
    if (!mxIsDouble(prhs[1]) || !mxIsDouble(prhs[2])) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df2T_f64:NotDouble", "Inputs must be double precision");
    }

    S.numStages = (uint8_t)mxGetScalar(prhs[0]);
    pCoeffs = (float64_t *)mxGetData(prhs[1]);
    S.pCoeffs = pCoeffs;
    pSrc = (float64_t *)mxGetData(prhs[2]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[2]);

    pState = (float64_t *)mxCalloc(2 * S.numStages, sizeof(float64_t));
    for (i = 0; i < 2 * S.numStages; i++) pState[i] = 0.0;
    S.pState = pState;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxDOUBLE_CLASS, mxREAL);
    pDst = (float64_t *)mxGetData(plhs[0]);

    arm_biquad_cascade_df2T_f64(&S, pSrc, pDst, blockSize);

    mxFree(pState);
}