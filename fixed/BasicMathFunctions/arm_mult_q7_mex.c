#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int8_t q7_t;

void arm_mult_q7(
    q7_t *pSrcA,
    q7_t *pSrcB,
    q7_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int16_t prod;

    blkCnt = blockSize;

    while (blkCnt > 0u)
	{
		prod = ((int16_t)*pSrcA++ * (int16_t)*pSrcB++) >> 7;

		if (prod > 127)
			prod = 127;
		else if (prod < -128)
			prod = -128;

		*pDst++ = (q7_t)prod;
		blkCnt--;
	}
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrcA, *pSrcB, *pDst;
    size_t blockSize;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_mult_q7:invalidInput", "2 inputs required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mult_q7:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mult_q7:invalidInputType", "Input 1 must be real int8.");
    if (!mxIsInt8(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_mult_q7:invalidInputType", "Input 2 must be real int8.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    if (mxGetNumberOfElements(prhs[1]) != blockSize)
        mexErrMsgIdAndTxt("arm_mult_q7:invalidInput", "Inputs must have the same size.");

    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT8_CLASS, mxREAL);
    pSrcA = (int8_t *)mxGetData(prhs[0]);
    pSrcB = (int8_t *)mxGetData(prhs[1]);
    pDst = (int8_t *)mxGetData(plhs[0]);

    arm_mult_q7(pSrcA, pSrcB, pDst, (uint32_t)blockSize);
}