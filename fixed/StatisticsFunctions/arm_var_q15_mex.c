#include <stdint.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_var_q15(
    q15_t *pSrc,
    uint32_t blockSize,
    q15_t *pResult)
{
    q31_t sum = 0;
    q31_t meanOfSquares, squareOfMean;
    uint32_t blkCnt;
    q63_t sumOfSquares = 0;
    q15_t in;

    if (blockSize == 1)
    {
        *pResult = 0;
        return;
    }

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        sumOfSquares += ((q63_t)in * in);
        sum += in;
        blkCnt--;
    }

    meanOfSquares = (q31_t)(sumOfSquares / (q63_t)(blockSize - 1u));
    squareOfMean = (q31_t)(((q63_t)sum * sum) / (q63_t)(blockSize * (blockSize - 1u)));

    *pResult = (q15_t)((meanOfSquares - squareOfMean) >> 15);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc;
    size_t blockSize;
    int16_t result;
    const mwSize dims[2] = {1, 1};

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_var_q15:invalidInput", "1 input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_var_q15:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_var_q15:invalidInputType", "Input must be real int16.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    pSrc = (int16_t *)mxGetData(prhs[0]);

    arm_var_q15(pSrc, (uint32_t)blockSize, &result);

    plhs[0] = mxCreateNumericArray(2, dims, mxINT16_CLASS, mxREAL);
    ((int16_t *)mxGetData(plhs[0]))[0] = result;
}