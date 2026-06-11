/* ----------------------------------------------------------------------
 * arm_fill_f32_mex.c
 * MEX wrapper for arm_fill_f32.
 * Usage: y = arm_fill_f32_mex(value, n)
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include "mex.h"

#include "arm_fill_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float value;
    float *pDst;
    uint32_t blockSize;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_fill_f32_mex:nrhs", "Two inputs required: value, n.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_fill_f32_mex:nlhs", "One output required.");
    if (!mxIsDouble(prhs[0]) || mxGetNumberOfElements(prhs[0]) != 1)
        mexErrMsgIdAndTxt("arm_fill_f32_mex:valueType", "Value must be a scalar double.");
    if (!mxIsDouble(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_fill_f32_mex:nType", "n must be a scalar double.");

    value = (float)mxGetScalar(prhs[0]);
    blockSize = (uint32_t)mxGetScalar(prhs[1]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    pDst = (float *)mxGetData(plhs[0]);

    arm_fill_f32(value, pDst, blockSize);
}