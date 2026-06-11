#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int8_t q7_t;

void arm_offset_q7(
    q7_t *pSrc,
    q7_t offset,
    q7_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int16_t val;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        val = (int16_t)*pSrc++ + (int16_t)offset;

        if (val > 127)
            val = 127;
        else if (val < -128)
            val = -128;

        *pDst++ = (q7_t)val;
        blkCnt--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrc, *pDst;
    int8_t offset;
    size_t blockSize;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_offset_q7:invalidInput", "2 inputs required (vector, scalar offset).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_offset_q7:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_offset_q7:invalidInputType", "Input 1 must be real int8.");
    if (!mxIsInt8(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_offset_q7:invalidInputType", "Input 2 must be a real int8 scalar.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT8_CLASS, mxREAL);
    pSrc = (int8_t *)mxGetData(prhs[0]);
    offset = *(int8_t *)mxGetData(prhs[1]);
    pDst = (int8_t *)mxGetData(plhs[0]);

    arm_offset_q7(pSrc, offset, pDst, (uint32_t)blockSize);
}