#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int16_t q15_t;

void arm_sub_q15(
    q15_t *pSrcA,
    q15_t *pSrcB,
    q15_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int32_t diff;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        diff = (int32_t)*pSrcA++ - (int32_t)*pSrcB++;

        if (diff > 32767)
            diff = 32767;
        else if (diff < -32768)
            diff = -32768;

        *pDst++ = (q15_t)diff;
        blkCnt--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrcA, *pSrcB, *pDst;
    size_t blockSize;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_sub_q15:invalidInput", "2 inputs required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_sub_q15:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_sub_q15:invalidInputType", "Input 1 must be real int16.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_sub_q15:invalidInputType", "Input 2 must be real int16.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    if (mxGetNumberOfElements(prhs[1]) != blockSize)
        mexErrMsgIdAndTxt("arm_sub_q15:invalidInput", "Inputs must have the same size.");

    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT16_CLASS, mxREAL);
    pSrcA = (int16_t *)mxGetData(prhs[0]);
    pSrcB = (int16_t *)mxGetData(prhs[1]);
    pDst = (int16_t *)mxGetData(plhs[0]);

    arm_sub_q15(pSrcA, pSrcB, pDst, (uint32_t)blockSize);
}