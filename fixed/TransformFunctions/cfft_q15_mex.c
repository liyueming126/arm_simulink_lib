#include "mex.h"
#include "cfft_q15_simulink.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 4)
    {
        mexErrMsgTxt("out = cfft_q15_mex(N,ifftFlag,bitRevFlag,inQ15_int16)");
    }
    uint32_t N = (uint32_t)mxGetScalar(prhs[0]);
    uint8_t ifft = (uint8_t)mxGetScalar(prhs[1]);
    uint8_t bitrev = (uint8_t)mxGetScalar(prhs[2]);
    uint32_t bufLen = N * 2;
    int16_t *pIn = (int16_t *)mxGetData(prhs[3]);
    q15_t *pOut = (q15_t *)mxMalloc(bufLen * sizeof(q15_t));

    cfft_q15_wrapper(N, ifft, bitrev, (q15_t *)pIn, pOut);

    plhs[0] = mxCreateNumericMatrix(1, bufLen, mxINT16_CLASS, mxREAL);
    int16_t *pMatOut = (int16_t *)mxGetData(plhs[0]);
    for (uint32_t i = 0; i < bufLen; i++)
        pMatOut[i] = pOut[i];

    mxFree(pOut);
}