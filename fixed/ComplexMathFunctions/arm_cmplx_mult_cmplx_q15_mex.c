#include "mex.h"
#include <stdint.h>

typedef int16_t q15_t;

void arm_cmplx_mult_cmplx_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t numSamples)
{
    q15_t a, b, c, d;

    while (numSamples > 0u)
    {
        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;

        *pDst++ = (q15_t)((int32_t)((((int32_t)a * (int32_t)c) >> 15) - (((int32_t)b * (int32_t)d) >> 15)));
        *pDst++ = (q15_t)((int32_t)((((int32_t)a * (int32_t)d) >> 15) + (((int32_t)b * (int32_t)c) >> 15)));

        numSamples--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 2) mexErrMsgIdAndTxt("cmplx_mult_cmplx_q15:input", "Two inputs required.");
    if (nlhs > 1) mexErrMsgIdAndTxt("cmplx_mult_cmplx_q15:output", "One output required.");

    if (!mxIsInt16(prhs[0]) || mxGetNumberOfDimensions(prhs[0]) != 2 || mxGetM(prhs[0]) != 1)
        mexErrMsgIdAndTxt("cmplx_mult_cmplx_q15:input", "Input A must be 1xN int16 vector.");
    if (!mxIsInt16(prhs[1]) || mxGetNumberOfDimensions(prhs[1]) != 2 || mxGetM(prhs[1]) != 1)
        mexErrMsgIdAndTxt("cmplx_mult_cmplx_q15:input", "Input B must be 1xN int16 vector.");

    mwSize nA = mxGetN(prhs[0]);
    mwSize nB = mxGetN(prhs[1]);

    if (nA != nB || nA % 2 != 0)
        mexErrMsgIdAndTxt("cmplx_mult_cmplx_q15:input", "Both inputs must be same length, even number of elements.");

    uint32_t numSamples = (uint32_t)(nA / 2);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)nA, mxINT16_CLASS, mxREAL);
    q15_t *srcA = (q15_t *)mxGetData(prhs[0]);
    q15_t *srcB = (q15_t *)mxGetData(prhs[1]);
    q15_t *dst  = (q15_t *)mxGetData(plhs[0]);

    arm_cmplx_mult_cmplx_q15(srcA, srcB, dst, numSamples);
}