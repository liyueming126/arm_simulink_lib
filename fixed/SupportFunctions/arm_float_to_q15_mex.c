#include "mex.h"
#include "matrix.h"
#include <stdint.h>

void arm_float_to_q15(const float *pSrc, int16_t *pDst, uint32_t blockSize)
{
    uint32_t i;
    for (i = 0u; i < blockSize; i++)
    {
        float v = pSrc[i] * 32768.0f;
        if (v > 32767.0f)
        {
            pDst[i] = 32767;
        }
        else if (v < -32768.0f)
        {
            pDst[i] = -32768;
        }
        else
        {
            int32_t iv = (int32_t)v;
            if (v < 0.0f && (float)iv != v) iv--;
            pDst[i] = (int16_t)iv;
        }
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc;
    int16_t *pDst;
    uint32_t blockSize;
    if (nrhs != 1) { mexErrMsgIdAndTxt("arm_float_to_q15:nrhs", "One input required."); }
    if (nlhs > 1)  { mexErrMsgIdAndTxt("arm_float_to_q15:nlhs", "One output required."); }
    if (!mxIsSingle(prhs[0])) { mexErrMsgIdAndTxt("arm_float_to_q15:notSingle", "Input must be single."); }
    pSrc = (float *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericMatrix(blockSize, 1, mxINT16_CLASS, mxREAL);
    pDst = (int16_t *)mxGetData(plhs[0]);
    if (nlhs > 0) { arm_float_to_q15(pSrc, pDst, blockSize); }
}