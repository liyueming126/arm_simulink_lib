#include "mex.h"
#include <stdint.h>

typedef int16_t q15_t;

void arm_cmplx_conj_q15(q15_t *pSrc, q15_t *pDst, uint32_t numSamples)
{
    while (numSamples > 0u)
    {
        *pDst++ = *pSrc++;
        q15_t in = *pSrc++;
        *pDst++ = (in == (q15_t)0x8000) ? (q15_t)0x7fff : (q15_t)(-in);
        numSamples--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 1) mexErrMsgIdAndTxt("cmplx_conj_q15:input", "One input required.");
    if (nlhs > 1) mexErrMsgIdAndTxt("cmplx_conj_q15:output", "One output required.");

    if (!mxIsInt16(prhs[0]) || mxGetNumberOfDimensions(prhs[0]) != 2 || mxGetM(prhs[0]) != 1)
        mexErrMsgIdAndTxt("cmplx_conj_q15:input", "Input must be 1xN int16 vector.");

    mwSize numElements = mxGetN(prhs[0]);
    if (numElements % 2 != 0)
        mexErrMsgIdAndTxt("cmplx_conj_q15:input", "Input length must be even (real,imag pairs).");

    uint32_t numSamples = (uint32_t)(numElements / 2);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)numElements, mxINT16_CLASS, mxREAL);
    q15_t *src = (q15_t *)mxGetData(prhs[0]);
    q15_t *dst = (q15_t *)mxGetData(plhs[0]);

    arm_cmplx_conj_q15(src, dst, numSamples);
}