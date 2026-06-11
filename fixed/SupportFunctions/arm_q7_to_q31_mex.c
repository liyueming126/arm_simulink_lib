#include "mex.h"
#include "matrix.h"
#include <stdint.h>

void arm_q7_to_q31(const int8_t *pSrc, int32_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        pDst[i] = (int32_t)pSrc[i] << 24;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrc;
    int32_t *pDst;
    uint32_t blockSize;
    if (nrhs != 1) { mexErrMsgIdAndTxt("arm_q7_to_q31:nrhs", "One input required."); }
    if (nlhs > 1)  { mexErrMsgIdAndTxt("arm_q7_to_q31:nlhs", "One output required."); }
    if (!mxIsInt8(prhs[0])) { mexErrMsgIdAndTxt("arm_q7_to_q31:notInt8", "Input must be int8."); }
    pSrc = (int8_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericMatrix(blockSize, 1, mxINT32_CLASS, mxREAL);
    pDst = (int32_t *)mxGetData(plhs[0]);
    if (nlhs > 0) { arm_q7_to_q31(pSrc, pDst, blockSize); }
}