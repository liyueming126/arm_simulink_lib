/* MEX wrapper for arm_fir_sparse_init_f32 */
#include "mex.h"
#include <string.h>

#include "arm_fir_sparse_init_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_fir_sparse_instance_f32 S;
    float32_t *pCoeffs, *pState;
    int32_t *pTapDelay;
    uint16_t numTaps, maxDelay;
    uint32_t blockSize;

    if (nrhs != 5) {
        mexErrMsgIdAndTxt("arm_fir_sparse_init_f32:InvalidInput", "Five inputs: numTaps, coefficients, tapDelays, maxDelay, blockSize");
    }
    if (nlhs < 1) {
        mexErrMsgIdAndTxt("arm_fir_sparse_init_f32:InvalidOutput", "At least one output required");
    }
    if (!mxIsSingle(prhs[1])) {
        mexErrMsgIdAndTxt("arm_fir_sparse_init_f32:NotSingle", "Coefficients must be single precision");
    }
    if (!mxIsInt32(prhs[2])) {
        mexErrMsgIdAndTxt("arm_fir_sparse_init_f32:NotInt32", "TapDelays must be int32");
    }

    numTaps = (uint16_t)mxGetScalar(prhs[0]);
    pCoeffs = (float32_t *)mxGetData(prhs[1]);
    pTapDelay = (int32_t *)mxGetData(prhs[2]);
    maxDelay = (uint16_t)mxGetScalar(prhs[3]);
    blockSize = (uint32_t)mxGetScalar(prhs[4]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(maxDelay + blockSize), mxSINGLE_CLASS, mxREAL);
    pState = (float32_t *)mxGetData(plhs[0]);
    memset(pState, 0, (maxDelay + blockSize) * sizeof(float32_t));

    S.numTaps = numTaps;
    S.pCoeffs = pCoeffs;
    S.pTapDelay = pTapDelay;
    S.maxDelay = maxDelay;
    S.stateIndex = 0;
    S.pState = pState;
}