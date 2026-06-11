/* ----------------------------------------------------------------------
 * arm_q31_to_float_mex.c
 * MEX wrapper for arm_q31_to_float.
 * Usage: y = arm_q31_to_float_mex(x)
 *   x: int32 input vector
 *   y: single output vector
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include "mex.h"

#include "arm_q31_to_float_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrc;
    float *pDst;
    uint32_t blockSize;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_q31_to_float_mex:nrhs", "One input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_q31_to_float_mex:nlhs", "One output required.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_q31_to_float_mex:notInt32", "Input must be real int32.");

    pSrc = (int32_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    pDst = (float *)mxGetData(plhs[0]);

    arm_q31_to_float(pSrc, pDst, blockSize);
}