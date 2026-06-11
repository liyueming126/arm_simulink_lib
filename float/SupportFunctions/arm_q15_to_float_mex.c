/* ----------------------------------------------------------------------
 * arm_q15_to_float_mex.c
 * MEX wrapper for arm_q15_to_float.
 * Usage: y = arm_q15_to_float_mex(x)
 *   x: int16 input vector
 *   y: single output vector
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include "mex.h"

#include "arm_q15_to_float_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc;
    float *pDst;
    uint32_t blockSize;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_q15_to_float_mex:nrhs", "One input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_q15_to_float_mex:nlhs", "One output required.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_q15_to_float_mex:notInt16", "Input must be real int16.");

    pSrc = (int16_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    pDst = (float *)mxGetData(plhs[0]);

    arm_q15_to_float(pSrc, pDst, blockSize);
}