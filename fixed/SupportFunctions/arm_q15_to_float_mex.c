#include "mex.h"
#include "matrix.h"
#include <stdint.h>

void arm_q15_to_float(const int16_t *pSrc, float *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        pDst[i] = (float)pSrc[i] / 32768.0f;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc;
    float *pDst;
    uint32_t blockSize;
    if (nrhs != 1) { mexErrMsgIdAndTxt("arm_q15_to_float:nrhs", "One input required."); }
    if (nlhs > 1)  { mexErrMsgIdAndTxt("arm_q15_to_float:nlhs", "One output required."); }
    if (!mxIsInt16(prhs[0])) { mexErrMsgIdAndTxt("arm_q15_to_float:notInt16", "Input must be int16."); }
    pSrc = (int16_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericMatrix(blockSize, 1, mxSINGLE_CLASS, mxREAL);
    pDst = (float *)mxGetData(plhs[0]);
    if (nlhs > 0) { arm_q15_to_float(pSrc, pDst, blockSize); }
}