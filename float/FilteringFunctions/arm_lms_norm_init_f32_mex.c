/* MEX wrapper for arm_lms_norm_init_f32 */
#include "mex.h"
#include <string.h>

#include "arm_lms_norm_init_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_lms_norm_instance_f32 S;
    float32_t *pCoeffs, *pState;
    uint16_t numTaps;
    float32_t mu;
    uint32_t blockSize;

    if (nrhs != 4) {
        mexErrMsgIdAndTxt("arm_lms_norm_init_f32:InvalidInput", "Four inputs: numTaps, coefficients, mu, blockSize");
    }
    if (nlhs < 1) {
        mexErrMsgIdAndTxt("arm_lms_norm_init_f32:InvalidOutput", "At least one output required");
    }
    if (!mxIsSingle(prhs[1])) {
        mexErrMsgIdAndTxt("arm_lms_norm_init_f32:NotSingle", "Coefficients must be single precision");
    }

    numTaps = (uint16_t)mxGetScalar(prhs[0]);
    pCoeffs = (float32_t *)mxGetData(prhs[1]);
    mu = (float32_t)mxGetScalar(prhs[2]);
    blockSize = (uint32_t)mxGetScalar(prhs[3]);

    S.numTaps = numTaps;
    S.pCoeffs = pCoeffs;
    S.mu = mu;
    S.energy = 0.0f;
    S.x0 = 0.0f;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(numTaps + blockSize - 1), mxSINGLE_CLASS, mxREAL);
    pState = (float32_t *)mxGetData(plhs[0]);
    memset(pState, 0, (numTaps + blockSize - 1) * sizeof(float32_t));
    S.pState = pState;
}