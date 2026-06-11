/* MEX wrapper for arm_iir_lattice_init_f32 */
#include "mex.h"
#include <string.h>

#include "arm_iir_lattice_init_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_iir_lattice_instance_f32 S;
    float32_t *pkCoeffs, *pvCoeffs, *pState;
    uint16_t numStages;
    uint32_t blockSize;

    if (nrhs != 4) {
        mexErrMsgIdAndTxt("arm_iir_lattice_init_f32:InvalidInput", "Four inputs: numStages, k coeffs, v coeffs, blockSize");
    }
    if (nlhs < 1) {
        mexErrMsgIdAndTxt("arm_iir_lattice_init_f32:InvalidOutput", "At least one output required");
    }
    if (!mxIsSingle(prhs[1]) || !mxIsSingle(prhs[2])) {
        mexErrMsgIdAndTxt("arm_iir_lattice_init_f32:NotSingle", "Coefficients must be single precision");
    }

    numStages = (uint16_t)mxGetScalar(prhs[0]);
    pkCoeffs = (float32_t *)mxGetData(prhs[1]);
    pvCoeffs = (float32_t *)mxGetData(prhs[2]);
    blockSize = (uint32_t)mxGetScalar(prhs[3]);

    S.numStages = numStages;
    S.pkCoeffs = pkCoeffs;
    S.pvCoeffs = pvCoeffs;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(numStages + blockSize), mxSINGLE_CLASS, mxREAL);
    pState = (float32_t *)mxGetData(plhs[0]);
    memset(pState, 0, (numStages + blockSize) * sizeof(float32_t));
    S.pState = pState;
}