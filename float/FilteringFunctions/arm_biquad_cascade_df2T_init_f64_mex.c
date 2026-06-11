/* MEX wrapper for arm_biquad_cascade_df2T_init_f64 */
#include "mex.h"
#include <string.h>

#include "arm_biquad_cascade_df2T_init_f64_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_biquad_cascade_df2T_instance_f64 S;
    float64_t *pCoeffs, *pState;
    uint8_t numStages;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df2T_init_f64:InvalidInput", "Two inputs: numStages, coefficients");
    }
    if (nlhs < 1) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df2T_init_f64:InvalidOutput", "At least one output required");
    }
    if (!mxIsDouble(prhs[1])) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df2T_init_f64:NotDouble", "Coefficients must be double precision");
    }

    numStages = (uint8_t)mxGetScalar(prhs[0]);
    pCoeffs = (float64_t *)mxGetData(prhs[1]);

    S.numStages = numStages;
    S.pCoeffs = pCoeffs;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(2 * numStages), mxDOUBLE_CLASS, mxREAL);
    pState = (float64_t *)mxGetData(plhs[0]);
    memset(pState, 0, (2u * (uint32_t)numStages) * sizeof(float64_t));
    S.pState = pState;
}