#include "mex.h"
#include "matrix.h"
#include <stdint.h>

void arm_q15_to_q31(const int16_t *pSrc, int32_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        pDst[i] = (int32_t)pSrc[i] << 16;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc;
    int32_t *pDst;
    uint32_t blockSize;
    if (nrhs != 1) { mexErrMsgIdAndTxt("arm_q15_to_q31:nrhs", "One input required."); }
    if (nlhs > 1)  { mexErrMsgIdAndTxt("arm_q15_to_q31:nlhs", "One output required."); }
    if (!mxIsInt16(prhs[0])) { mexErrMsgIdAndTxt("arm_q15_to_q31:notInt16", "Input must be int16."); }
    pSrc = (int16_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericMatrix(blockSize, 1, mxINT32_CLASS, mxREAL);
    pDst = (int32_t *)mxGetData(plhs[0]);
    if (nlhs > 0) { arm_q15_to_q31(pSrc, pDst, blockSize); }
}