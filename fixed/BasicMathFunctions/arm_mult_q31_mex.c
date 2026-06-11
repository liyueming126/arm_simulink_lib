#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int32_t q31_t;

void arm_mult_q31(
    q31_t *pSrcA,
    q31_t *pSrcB,
    q31_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int64_t prod;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        prod = (int64_t)*pSrcA++ * (int64_t)*pSrcB++;
        prod = prod >> 31;

        if (prod > 2147483647LL)
            prod = 2147483647LL;
        else if (prod < -2147483648LL)
            prod = -2147483648LL;

        *pDst++ = (q31_t)prod;
        blkCnt--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrcA, *pSrcB, *pDst;
    size_t blockSize;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_mult_q31:invalidInput", "2 inputs required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mult_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mult_q31:invalidInputType", "Input 1 must be real int32.");
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_mult_q31:invalidInputType", "Input 2 must be real int32.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    if (mxGetNumberOfElements(prhs[1]) != blockSize)
        mexErrMsgIdAndTxt("arm_mult_q31:invalidInput", "Inputs must have the same size.");

    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT32_CLASS, mxREAL);
    pSrcA = (int32_t *)mxGetData(prhs[0]);
    pSrcB = (int32_t *)mxGetData(prhs[1]);
    pDst = (int32_t *)mxGetData(plhs[0]);

    arm_mult_q31(pSrcA, pSrcB, pDst, (uint32_t)blockSize);
}