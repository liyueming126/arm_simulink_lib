#include "mex.h"
#include "matrix.h"
#include <stdint.h>

void arm_float_to_q31(const float *pSrc, int32_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        double v = (double)pSrc[i] * 2147483648.0;
        if (v > 2147483647.0)
        {
            pDst[i] = 2147483647;
        }
        else if (v < -2147483648.0)
        {
            pDst[i] = (int32_t)(-2147483648);
        }
        else
        {
            int64_t iv = (int64_t)v;
            if (v < 0.0 && (double)iv != v) iv--;
            pDst[i] = (int32_t)iv;
        }
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc;
    int32_t *pDst;
    uint32_t blockSize;
    if (nrhs != 1) { mexErrMsgIdAndTxt("arm_float_to_q31:nrhs", "One input required."); }
    if (nlhs > 1)  { mexErrMsgIdAndTxt("arm_float_to_q31:nlhs", "One output required."); }
    if (!mxIsSingle(prhs[0])) { mexErrMsgIdAndTxt("arm_float_to_q31:notSingle", "Input must be single."); }
    pSrc = (float *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericMatrix(blockSize, 1, mxINT32_CLASS, mxREAL);
    pDst = (int32_t *)mxGetData(plhs[0]);
    if (nlhs > 0) { arm_float_to_q31(pSrc, pDst, blockSize); }
}