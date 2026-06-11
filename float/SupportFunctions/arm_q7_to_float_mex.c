/* ----------------------------------------------------------------------
 * arm_q7_to_float_mex.c
 * MEX wrapper for arm_q7_to_float.
 * Usage: y = arm_q7_to_float_mex(x)
 *   x: int8 input vector
 *   y: single output vector
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include "mex.h"

#include "arm_q7_to_float_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrc;
    float *pDst;
    uint32_t blockSize;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_q7_to_float_mex:nrhs", "One input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_q7_to_float_mex:nlhs", "One output required.");
    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_q7_to_float_mex:notInt8", "Input must be real int8.");

    pSrc = (int8_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    pDst = (float *)mxGetData(plhs[0]);

    arm_q7_to_float(pSrc, pDst, blockSize);
}