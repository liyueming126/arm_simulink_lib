/* MEX wrapper for arm_iir_lattice_f32 */
#include "mex.h"
#include <string.h>

#include "arm_iir_lattice_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_iir_lattice_instance_f32 S;
    float32_t *pSrc, *pDst, *pkCoeffs, *pvCoeffs, *pState;
    uint32_t blockSize;
    mwSize i;

    if (nrhs != 3) {
        mexErrMsgIdAndTxt("arm_iir_lattice_f32:InvalidInput", "Three inputs: reflection coeffs (k), ladder coeffs (v), input signal");
    }
    if (nlhs > 1) {
        mexErrMsgIdAndTxt("arm_iir_lattice_f32:InvalidOutput", "One output required");
    }
    if (!mxIsSingle(prhs[0]) || !mxIsSingle(prhs[1]) || !mxIsSingle(prhs[2])) {
        mexErrMsgIdAndTxt("arm_iir_lattice_f32:NotSingle", "Inputs must be single precision");
    }

    pkCoeffs = (float32_t *)mxGetData(prhs[0]);
    pvCoeffs = (float32_t *)mxGetData(prhs[1]);
    pSrc = (float32_t *)mxGetData(prhs[2]);
    S.numStages = (uint16_t)mxGetNumberOfElements(prhs[0]);
    S.pkCoeffs = pkCoeffs;
    S.pvCoeffs = pvCoeffs;
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[2]);

    pState = (float32_t *)mxCalloc((mwSize)(S.numStages + blockSize), sizeof(float32_t));
    for (i = 0; i < (mwSize)(S.numStages + blockSize); i++) pState[i] = 0.0f;
    S.pState = pState;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    pDst = (float32_t *)mxGetData(plhs[0]);

    arm_iir_lattice_f32(&S, pSrc, pDst, blockSize);

    mxFree(pState);
}