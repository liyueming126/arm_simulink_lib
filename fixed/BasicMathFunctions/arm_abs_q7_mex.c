#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int8_t q7_t;

void arm_abs_q7(
    q7_t *pSrc,
    q7_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    q7_t in;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        *pDst++ = (in > 0) ? in : ((in == INT8_MIN) ? INT8_MAX : (q7_t)(-in));
        blkCnt--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrc, *pDst;
    size_t blockSize;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_abs_q7:invalidInput", "1 input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_abs_q7:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_abs_q7:invalidInputType", "Input must be real int8.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT8_CLASS, mxREAL);
    pSrc = (int8_t *)mxGetData(prhs[0]);
    pDst = (int8_t *)mxGetData(plhs[0]);

    arm_abs_q7(pSrc, pDst, (uint32_t)blockSize);
}