#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int32_t q31_t;

void arm_abs_q31(
    q31_t *pSrc,
    q31_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    q31_t in;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : (q31_t)(-in));
        blkCnt--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrc, *pDst;
    size_t blockSize;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_abs_q31:invalidInput", "1 input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_abs_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_abs_q31:invalidInputType", "Input must be real int32.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT32_CLASS, mxREAL);
    pSrc = (int32_t *)mxGetData(prhs[0]);
    pDst = (int32_t *)mxGetData(plhs[0]);

    arm_abs_q31(pSrc, pDst, (uint32_t)blockSize);
}