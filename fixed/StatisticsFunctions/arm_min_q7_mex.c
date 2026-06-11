#include <stdint.h>
#include "mex.h"

typedef int8_t q7_t;

void arm_min_q7(
    q7_t *pSrc,
    uint32_t blockSize,
    q7_t *pResult,
    uint32_t *pIndex)
{
    q7_t minVal, out;
    uint32_t blkCnt, outIndex;

    outIndex = 0u;
    out = *pSrc++;

    blkCnt = (blockSize - 1u);

    while (blkCnt > 0u)
    {
        minVal = *pSrc++;

        if (out > minVal)
        {
            out = minVal;
            outIndex = blockSize - blkCnt;
        }

        blkCnt--;
    }

    *pResult = out;
    *pIndex = outIndex;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrc;
    size_t blockSize;
    int8_t resultVal;
    uint32_t resultIdx;
    const mwSize dims[2] = {1, 1};

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_min_q7:invalidInput", "1 input required.");
    if (nlhs > 2)
        mexErrMsgIdAndTxt("arm_min_q7:invalidOutput", "At most 2 outputs allowed.");
    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_min_q7:invalidInputType", "Input must be real int8.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    pSrc = (int8_t *)mxGetData(prhs[0]);

    arm_min_q7(pSrc, (uint32_t)blockSize, &resultVal, &resultIdx);

    if (nlhs >= 1)
    {
        plhs[0] = mxCreateNumericArray(2, dims, mxINT8_CLASS, mxREAL);
        ((int8_t *)mxGetData(plhs[0]))[0] = resultVal;
    }
    if (nlhs >= 2)
        plhs[1] = mxCreateDoubleScalar((double)(resultIdx + 1u));
}