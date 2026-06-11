/* ----------------------------------------------------------------------
 * arm_float_to_q7_mex.c
 * MEX wrapper for arm_float_to_q7.
 * Usage: y = arm_float_to_q7_mex(x)
 *   x: single input vector
 *   y: int8 output vector
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include "mex.h"

#include "arm_float_to_q7_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc;
    int8_t *pDst;
    uint32_t blockSize;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_float_to_q7_mex:nrhs", "One input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_float_to_q7_mex:nlhs", "One output required.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_float_to_q7_mex:notSingle", "Input must be real single.");

    pSrc = (float *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxINT8_CLASS, mxREAL);
    pDst = (int8_t *)mxGetData(plhs[0]);

    arm_float_to_q7(pSrc, pDst, blockSize);
}