#include "mex.h"
#include <stdint.h>

typedef int32_t q31_t;

void arm_cmplx_conj_q31(q31_t *pSrc, q31_t *pDst, uint32_t numSamples)
{
    while (numSamples > 0u)
    {
        *pDst++ = *pSrc++;
        q31_t in = *pSrc++;
        *pDst++ = (in == (q31_t)INT32_MIN) ? (q31_t)INT32_MAX : (q31_t)(-in);
        numSamples--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 1) mexErrMsgIdAndTxt("cmplx_conj_q31:input", "One input required.");
    if (nlhs > 1) mexErrMsgIdAndTxt("cmplx_conj_q31:output", "One output required.");

    if (!mxIsInt32(prhs[0]) || mxGetNumberOfDimensions(prhs[0]) != 2 || mxGetM(prhs[0]) != 1)
        mexErrMsgIdAndTxt("cmplx_conj_q31:input", "Input must be 1xN int32 vector.");

    mwSize numElements = mxGetN(prhs[0]);
    if (numElements % 2 != 0)
        mexErrMsgIdAndTxt("cmplx_conj_q31:input", "Input length must be even (real,imag pairs).");

    uint32_t numSamples = (uint32_t)(numElements / 2);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)numElements, mxINT32_CLASS, mxREAL);
    q31_t *src = (q31_t *)mxGetData(prhs[0]);
    q31_t *dst = (q31_t *)mxGetData(plhs[0]);

    arm_cmplx_conj_q31(src, dst, numSamples);
}