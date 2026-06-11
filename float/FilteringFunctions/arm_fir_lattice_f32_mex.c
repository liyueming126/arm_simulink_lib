/* MEX wrapper for arm_fir_lattice_f32 */
#include "mex.h"
#include <string.h>

#include "arm_fir_lattice_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_fir_lattice_instance_f32 S;
    float32_t *pSrc, *pDst, *pCoeffs, *pState;
    uint32_t blockSize;
    mwSize i;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("arm_fir_lattice_f32:InvalidInput", "Two inputs: coefficients, input signal");
    }
    if (nlhs > 1) {
        mexErrMsgIdAndTxt("arm_fir_lattice_f32:InvalidOutput", "One output required");
    }
    if (!mxIsSingle(prhs[0]) || !mxIsSingle(prhs[1])) {
        mexErrMsgIdAndTxt("arm_fir_lattice_f32:NotSingle", "Inputs must be single precision");
    }

    pCoeffs = (float32_t *)mxGetData(prhs[0]);
    pSrc = (float32_t *)mxGetData(prhs[1]);
    S.numStages = (uint16_t)mxGetNumberOfElements(prhs[0]);
    S.pCoeffs = pCoeffs;
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[1]);

    pState = (float32_t *)mxCalloc(S.numStages, sizeof(float32_t));
    for (i = 0; i < S.numStages; i++) pState[i] = 0.0f;
    S.pState = pState;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    pDst = (float32_t *)mxGetData(plhs[0]);

    arm_fir_lattice_f32(&S, pSrc, pDst, blockSize);

    mxFree(pState);
}