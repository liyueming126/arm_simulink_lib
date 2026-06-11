/* MEX wrapper for arm_fir_lattice_init_f32 */
#include "mex.h"
#include <string.h>

#include "arm_fir_lattice_init_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_fir_lattice_instance_f32 S;
    float32_t *pCoeffs, *pState;
    uint16_t numStages;
    mwSize i;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("arm_fir_lattice_init_f32:InvalidInput", "Two inputs: numStages, coefficients");
    }
    if (nlhs < 1) {
        mexErrMsgIdAndTxt("arm_fir_lattice_init_f32:InvalidOutput", "At least one output required");
    }
    if (!mxIsSingle(prhs[1])) {
        mexErrMsgIdAndTxt("arm_fir_lattice_init_f32:NotSingle", "Coefficients must be single precision");
    }

    numStages = (uint16_t)mxGetScalar(prhs[0]);
    pCoeffs = (float32_t *)mxGetData(prhs[1]);

    S.numStages = numStages;
    S.pCoeffs = pCoeffs;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)numStages, mxSINGLE_CLASS, mxREAL);
    pState = (float32_t *)mxGetData(plhs[0]);
    for (i = 0; i < numStages; i++) pState[i] = 0.0f;
    S.pState = pState;
}