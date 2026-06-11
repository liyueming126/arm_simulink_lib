#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int16_t q15_t;

void arm_negate_q15(
    q15_t *pSrc,
    q15_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    q15_t in;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        *pDst++ = (in == INT16_MIN) ? INT16_MAX : (q15_t)(-in);
        blkCnt--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc, *pDst;
    size_t blockSize;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_negate_q15:invalidInput", "1 input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_negate_q15:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_negate_q15:invalidInputType", "Input must be real int16.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT16_CLASS, mxREAL);
    pSrc = (int16_t *)mxGetData(prhs[0]);
    pDst = (int16_t *)mxGetData(plhs[0]);

    arm_negate_q15(pSrc, pDst, (uint32_t)blockSize);
}