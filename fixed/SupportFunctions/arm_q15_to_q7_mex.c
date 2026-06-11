#include "mex.h"
#include "matrix.h"
#include <stdint.h>

void arm_q15_to_q7(const int16_t *pSrc, int8_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        pDst[i] = (int8_t)(pSrc[i] >> 8);
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc;
    int8_t *pDst;
    uint32_t blockSize;
    if (nrhs != 1) { mexErrMsgIdAndTxt("arm_q15_to_q7:nrhs", "One input required."); }
    if (nlhs > 1)  { mexErrMsgIdAndTxt("arm_q15_to_q7:nlhs", "One output required."); }
    if (!mxIsInt16(prhs[0])) { mexErrMsgIdAndTxt("arm_q15_to_q7:notInt16", "Input must be int16."); }
    pSrc = (int16_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericMatrix(blockSize, 1, mxINT8_CLASS, mxREAL);
    pDst = (int8_t *)mxGetData(plhs[0]);
    if (nlhs > 0) { arm_q15_to_q7(pSrc, pDst, blockSize); }
}