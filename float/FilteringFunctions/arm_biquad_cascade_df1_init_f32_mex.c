/* MEX wrapper for arm_biquad_cascade_df1_init_f32 */
#include "mex.h"
#include <string.h>

#include "arm_biquad_cascade_df1_init_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_biquad_casd_df1_inst_f32 S;
    float32_t *pCoeffs, *pState;
    uint8_t numStages;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df1_init_f32:InvalidInput", "Two inputs: numStages, coefficients");
    }
    if (nlhs < 1) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df1_init_f32:InvalidOutput", "At least one output required");
    }
    if (!mxIsSingle(prhs[1])) {
        mexErrMsgIdAndTxt("arm_biquad_cascade_df1_init_f32:NotSingle", "Coefficients must be single precision");
    }

    numStages = (uint8_t)mxGetScalar(prhs[0]);
    pCoeffs = (float32_t *)mxGetData(prhs[1]);

    S.numStages = numStages;
    S.pCoeffs = pCoeffs;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(4 * numStages), mxSINGLE_CLASS, mxREAL);
    pState = (float32_t *)mxGetData(plhs[0]);
    memset(pState, 0, (4u * (uint32_t)numStages) * sizeof(float32_t));
    S.pState = pState;
}