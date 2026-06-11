/* ----------------------------------------------------------------------
 * arm_copy_f32_mex.c
 * MEX wrapper for arm_copy_f32.
 * Usage: y = arm_copy_f32_mex(x)
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include "mex.h"

/* Include the self-contained implementation */
#include "arm_copy_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, *pDst;
    uint32_t blockSize;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_copy_f32_mex:nrhs", "One input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_copy_f32_mex:nlhs", "One output required.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_copy_f32_mex:notSingle", "Input must be real single.");

    pSrc = (float *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxSINGLE_CLASS, mxREAL);
    pDst = (float *)mxGetData(plhs[0]);

    arm_copy_f32(pSrc, pDst, blockSize);
}