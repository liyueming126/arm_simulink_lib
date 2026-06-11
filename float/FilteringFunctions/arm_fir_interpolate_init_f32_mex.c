/* MEX wrapper for arm_fir_interpolate_init_f32 */
#include "mex.h"
#include <string.h>

#include "arm_fir_interpolate_init_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_fir_interpolate_instance_f32 S;
    float32_t *pCoeffs, *pState;
    uint8_t L;
    uint16_t numTaps;
    uint32_t blockSize;
    arm_status status;
    mwSize stateSize;

    if (nrhs != 4) {
        mexErrMsgIdAndTxt("arm_fir_interpolate_init_f32:InvalidInput", "Four inputs: L, numTaps, coefficients, blockSize");
    }
    if (nlhs < 2) {
        mexErrMsgIdAndTxt("arm_fir_interpolate_init_f32:InvalidOutput", "Two outputs: status, state buffer");
    }
    if (!mxIsSingle(prhs[2])) {
        mexErrMsgIdAndTxt("arm_fir_interpolate_init_f32:NotSingle", "Coefficients must be single precision");
    }

    L = (uint8_t)mxGetScalar(prhs[0]);
    numTaps = (uint16_t)mxGetScalar(prhs[1]);
    blockSize = (uint32_t)mxGetScalar(prhs[3]);
    pCoeffs = (float32_t *)mxGetData(prhs[2]);

    stateSize = (mwSize)(blockSize + (numTaps / L) - 1);
    plhs[1] = mxCreateNumericMatrix(1, stateSize, mxSINGLE_CLASS, mxREAL);
    pState = (float32_t *)mxGetData(plhs[1]);

    status = arm_fir_interpolate_init_f32(&S, L, numTaps, pCoeffs, pState, blockSize);

    plhs[0] = mxCreateDoubleScalar((double)status);
}