#include <stdint.h>
#include "mex.h"

typedef int32_t q31_t;

void arm_min_q31(
    q31_t *pSrc,
    uint32_t blockSize,
    q31_t *pResult,
    uint32_t *pIndex)
{
    q31_t minVal, out;
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
    int32_t *pSrc;
    size_t blockSize;
    int32_t resultVal;
    uint32_t resultIdx;
    const mwSize dims[2] = {1, 1};

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_min_q31:invalidInput", "1 input required.");
    if (nlhs > 2)
        mexErrMsgIdAndTxt("arm_min_q31:invalidOutput", "At most 2 outputs allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_min_q31:invalidInputType", "Input must be real int32.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    pSrc = (int32_t *)mxGetData(prhs[0]);

    arm_min_q31(pSrc, (uint32_t)blockSize, &resultVal, &resultIdx);

    if (nlhs >= 1)
    {
        plhs[0] = mxCreateNumericArray(2, dims, mxINT32_CLASS, mxREAL);
        ((int32_t *)mxGetData(plhs[0]))[0] = resultVal;
    }
    if (nlhs >= 2)
        plhs[1] = mxCreateDoubleScalar((double)(resultIdx + 1u));
}