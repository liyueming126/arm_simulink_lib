#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int32_t q31_t;

void arm_shift_q31(
    q31_t *pSrc,
    int8_t shiftBits,
    q31_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;

    blkCnt = blockSize;

    if (shiftBits >= 0)
    {
        while (blkCnt > 0u)
        {
            int64_t val = (int64_t)*pSrc++ << shiftBits;

            if (val > 2147483647LL)
                val = 2147483647LL;
            else if (val < -2147483648LL)
                val = -2147483648LL;

            *pDst++ = (q31_t)val;
            blkCnt--;
        }
    }
    else
    {
        while (blkCnt > 0u)
        {
            *pDst++ = *pSrc++ >> (-shiftBits);
            blkCnt--;
        }
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrc, *pDst;
    int8_t shiftBits;
    size_t blockSize;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_shift_q31:invalidInput", "2 inputs required (vector, shiftBits).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_shift_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_shift_q31:invalidInputType", "Input 1 must be real int32.");
    if (!mxIsInt8(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_shift_q31:invalidInputType", "Input 2 (shiftBits) must be a real int8 scalar.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT32_CLASS, mxREAL);
    pSrc = (int32_t *)mxGetData(prhs[0]);
    shiftBits = *(int8_t *)mxGetData(prhs[1]);
    pDst = (int32_t *)mxGetData(plhs[0]);

    arm_shift_q31(pSrc, shiftBits, pDst, (uint32_t)blockSize);
}