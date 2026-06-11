#include <stdint.h>
#include "mex.h"

typedef int16_t q15_t;

void arm_max_q15(
    q15_t *pSrc,
    uint32_t blockSize,
    q15_t *pResult,
    uint32_t *pIndex)
{
    q15_t maxVal, out;
    uint32_t blkCnt, outIndex;

    outIndex = 0u;
    out = *pSrc++;

    blkCnt = (blockSize - 1u);

    while (blkCnt > 0u)
    {
        maxVal = *pSrc++;

        if (out < maxVal)
        {
            out = maxVal;
            outIndex = blockSize - blkCnt;
        }

        blkCnt--;
    }

    *pResult = out;
    *pIndex = outIndex;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc;
    size_t blockSize;
    int16_t resultVal;
    uint32_t resultIdx;
    const mwSize dims[2] = {1, 1};

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_max_q15:invalidInput", "1 input required.");
    if (nlhs > 2)
        mexErrMsgIdAndTxt("arm_max_q15:invalidOutput", "At most 2 outputs allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_max_q15:invalidInputType", "Input must be real int16.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    pSrc = (int16_t *)mxGetData(prhs[0]);

    arm_max_q15(pSrc, (uint32_t)blockSize, &resultVal, &resultIdx);

    if (nlhs >= 1)
    {
        plhs[0] = mxCreateNumericArray(2, dims, mxINT16_CLASS, mxREAL);
        ((int16_t *)mxGetData(plhs[0]))[0] = resultVal;
    }
    if (nlhs >= 2)
        plhs[1] = mxCreateDoubleScalar((double)(resultIdx + 1u));
}