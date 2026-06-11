#include <stdint.h>
#include "mex.h"

typedef float float32_t;

#include "arm_dot_prod_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrcA, *pSrcB, result;
    size_t blockSize;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_dot_prod_f32:invalidInput", "2 inputs required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_dot_prod_f32:invalidOutput", "At most 1 output allowed.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_dot_prod_f32:invalidInputType", "Input 1 must be real single.");
    if (!mxIsSingle(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_dot_prod_f32:invalidInputType", "Input 2 must be real single.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    if (mxGetNumberOfElements(prhs[1]) != blockSize)
        mexErrMsgIdAndTxt("arm_dot_prod_f32:invalidInput", "Inputs must have the same size.");

    pSrcA = (float *)mxGetData(prhs[0]);
    pSrcB = (float *)mxGetData(prhs[1]);

    arm_dot_prod_f32(pSrcA, pSrcB, (uint32_t)blockSize, &result);

    plhs[0] = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
    *(float *)mxGetData(plhs[0]) = result;
}