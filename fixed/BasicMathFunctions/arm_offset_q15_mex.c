#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int16_t q15_t;

void arm_offset_q15(
    q15_t *pSrc,
    q15_t offset,
    q15_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int32_t val;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        val = (int32_t)*pSrc++ + (int32_t)offset;

        if (val > 32767)
            val = 32767;
        else if (val < -32768)
            val = -32768;

        *pDst++ = (q15_t)val;
        blkCnt--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc, *pDst;
    int16_t offset;
    size_t blockSize;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_offset_q15:invalidInput", "2 inputs required (vector, scalar offset).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_offset_q15:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_offset_q15:invalidInputType", "Input 1 must be real int16.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_offset_q15:invalidInputType", "Input 2 must be a real int16 scalar.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT16_CLASS, mxREAL);
    pSrc = (int16_t *)mxGetData(prhs[0]);
    offset = *(int16_t *)mxGetData(prhs[1]);
    pDst = (int16_t *)mxGetData(plhs[0]);

    arm_offset_q15(pSrc, offset, pDst, (uint32_t)blockSize);
}