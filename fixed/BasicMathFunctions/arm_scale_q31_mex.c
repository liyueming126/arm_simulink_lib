#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int32_t q31_t;

void arm_scale_q31(
    q31_t *pSrc,
    q31_t scaleFract,
    int8_t shift,
    q31_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int64_t val;
    int8_t kShift;

    blkCnt = blockSize;
    kShift = 31 - shift;

    while (blkCnt > 0u)
    {
        val = ((int64_t)*pSrc++ * (int64_t)scaleFract) >> kShift;

        if (val > 2147483647LL)
            val = 2147483647LL;
        else if (val < -2147483648LL)
            val = -2147483648LL;

        *pDst++ = (q31_t)val;
        blkCnt--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrc, *pDst;
    int32_t scaleFract;
    int8_t shift;
    size_t blockSize;

    if (nrhs != 3)
        mexErrMsgIdAndTxt("arm_scale_q31:invalidInput", "3 inputs required (vector, scaleFract, shift).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_scale_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_scale_q31:invalidInputType", "Input 1 must be real int32.");
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_scale_q31:invalidInputType", "Input 2 (scaleFract) must be a real int32 scalar.");
    if (!mxIsInt8(prhs[2]) || mxIsComplex(prhs[2]) || mxGetNumberOfElements(prhs[2]) != 1)
        mexErrMsgIdAndTxt("arm_scale_q31:invalidInputType", "Input 3 (shift) must be a real int8 scalar.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT32_CLASS, mxREAL);
    pSrc = (int32_t *)mxGetData(prhs[0]);
    scaleFract = *(int32_t *)mxGetData(prhs[1]);
    shift = *(int8_t *)mxGetData(prhs[2]);
    pDst = (int32_t *)mxGetData(plhs[0]);

    arm_scale_q31(pSrc, scaleFract, shift, pDst, (uint32_t)blockSize);
}