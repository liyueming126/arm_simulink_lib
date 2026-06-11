#include "mex.h"
#include "matrix.h"
#include <stdint.h>

void arm_fill_q7(int8_t value, int8_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        pDst[i] = value;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t value;
    int8_t *pDst;
    uint32_t blockSize;
    if (nrhs != 2) { mexErrMsgIdAndTxt("arm_fill_q7:nrhs", "Two inputs required: value, blockSize."); }
    if (nlhs > 1)  { mexErrMsgIdAndTxt("arm_fill_q7:nlhs", "One output required."); }
    value = (int8_t)mxGetScalar(prhs[0]);
    blockSize = (uint32_t)mxGetScalar(prhs[1]);
    plhs[0] = mxCreateNumericMatrix(blockSize, 1, mxINT8_CLASS, mxREAL);
    pDst = (int8_t *)mxGetData(plhs[0]);
    if (nlhs > 0) { arm_fill_q7(value, pDst, blockSize); }
}