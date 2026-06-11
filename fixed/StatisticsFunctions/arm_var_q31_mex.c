#include <stdint.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_var_q31(
    q31_t *pSrc,
    uint32_t blockSize,
    q31_t *pResult)
{
    q63_t sum = 0;
    q63_t meanOfSquares, squareOfMean;
    q31_t in;
    uint32_t blkCnt;
    q63_t sumOfSquares = 0;

    if (blockSize == 1)
    {
        *pResult = 0;
        return;
    }

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++ >> 8;
        sumOfSquares += ((q63_t)in * in);
        sum += in;
        blkCnt--;
    }

    meanOfSquares = sumOfSquares / (q63_t)(blockSize - 1u);
    squareOfMean = sum * sum / (q63_t)(blockSize * (blockSize - 1u));

    *pResult = (q31_t)((meanOfSquares - squareOfMean) >> 15);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrc;
    size_t blockSize;
    int32_t result;
    const mwSize dims[2] = {1, 1};

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_var_q31:invalidInput", "1 input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_var_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_var_q31:invalidInputType", "Input must be real int32.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    pSrc = (int32_t *)mxGetData(prhs[0]);

    arm_var_q31(pSrc, (uint32_t)blockSize, &result);

    plhs[0] = mxCreateNumericArray(2, dims, mxINT32_CLASS, mxREAL);
    ((int32_t *)mxGetData(plhs[0]))[0] = result;
}