/* MEX wrapper for arm_fir_init_f32 */
#include "mex.h"
#include <string.h>

#include "arm_fir_init_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_fir_instance_f32 S;
    float32_t *pCoeffs, *pState;
    uint16_t numTaps;
    uint32_t blockSize;
    mwSize stateSize;

    if (nrhs != 3) {
        mexErrMsgIdAndTxt("arm_fir_init_f32:InvalidInput", "Three inputs required: numTaps, coefficients, blockSize");
    }
    if (nlhs < 2) {
        mexErrMsgIdAndTxt("arm_fir_init_f32:InvalidOutput", "Two outputs required: instance struct, state buffer");
    }
    if (!mxIsSingle(prhs[1])) {
        mexErrMsgIdAndTxt("arm_fir_init_f32:NotSingle", "Coefficients must be single precision");
    }

    numTaps = (uint16_t)mxGetScalar(prhs[0]);
    blockSize = (uint32_t)mxGetScalar(prhs[2]);
    pCoeffs = (float32_t *)mxGetData(prhs[1]);

    stateSize = (mwSize)(numTaps + blockSize - 1);
    plhs[1] = mxCreateNumericMatrix(1, stateSize, mxSINGLE_CLASS, mxREAL);
    pState = (float32_t *)mxGetData(plhs[1]);

    arm_fir_init_f32(&S, numTaps, pCoeffs, pState, blockSize);

    /* Return instance struct fields */
    plhs[0] = mxCreateNumericMatrix(1, 3, mxDOUBLE_CLASS, mxREAL);
    double *inst = (double *)mxGetData(plhs[0]);
    inst[0] = (double)S.numTaps;
    inst[1] = 0.0; /* pState pointer placeholder */
    inst[2] = 0.0; /* pCoeffs pointer placeholder */
}