#include <stdint.h>
#include "mex.h"

typedef float float32_t;

#include "arm_negate_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, *pDst;
    size_t blockSize;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_negate_f32:invalidInput", "1 input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_negate_f32:invalidOutput", "At most 1 output allowed.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_negate_f32:invalidInputType", "Input must be real single.");

    blockSize = mxGetNumberOfElements(prhs[0]);

    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]),
        mxGetDimensions(prhs[0]), mxSINGLE_CLASS, mxREAL);
    pSrc = (float *)mxGetData(prhs[0]);
    pDst = (float *)mxGetData(plhs[0]);

    arm_negate_f32(pSrc, pDst, (uint32_t)blockSize);
}