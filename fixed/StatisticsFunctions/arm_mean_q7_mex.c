#include <stdint.h>
#include "mex.h"

typedef int8_t q7_t;
typedef int32_t q31_t;

void arm_mean_q7(
    q7_t *pSrc,
    uint32_t blockSize,
    q7_t *pResult)
{
    q31_t sum = 0;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum += *pSrc++;
        blkCnt--;
    }

    *pResult = (q7_t)(sum / (q31_t)blockSize);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrc;
    size_t blockSize;
    int8_t result;
    const mwSize dims[2] = {1, 1};

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_mean_q7:invalidInput", "1 input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mean_q7:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mean_q7:invalidInputType", "Input must be real int8.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    pSrc = (int8_t *)mxGetData(prhs[0]);

    arm_mean_q7(pSrc, (uint32_t)blockSize, &result);

    plhs[0] = mxCreateNumericArray(2, dims, mxINT8_CLASS, mxREAL);
    ((int8_t *)mxGetData(plhs[0]))[0] = result;
}