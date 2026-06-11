#include "mex.h"
#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_cmplx_mult_cmplx_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t numSamples)
{
    q31_t a, b, c, d;
    q31_t mul1, mul2, mul3, mul4;

    while (numSamples > 0u)
    {
        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;

        mul1 = (q31_t)(((q63_t)a * c) >> 32);
        mul2 = (q31_t)(((q63_t)b * d) >> 32);
        mul3 = (q31_t)(((q63_t)a * d) >> 32);
        mul4 = (q31_t)(((q63_t)b * c) >> 32);
        mul1 = (mul1 >> 1);
        mul2 = (mul2 >> 1);
        mul3 = (mul3 >> 1);
        mul4 = (mul4 >> 1);

        *pDst++ = mul1 - mul2;
        *pDst++ = mul3 + mul4;

        numSamples--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 2) mexErrMsgIdAndTxt("cmplx_mult_cmplx_q31:input", "Two inputs required.");
    if (nlhs > 1) mexErrMsgIdAndTxt("cmplx_mult_cmplx_q31:output", "One output required.");

    if (!mxIsInt32(prhs[0]) || mxGetNumberOfDimensions(prhs[0]) != 2 || mxGetM(prhs[0]) != 1)
        mexErrMsgIdAndTxt("cmplx_mult_cmplx_q31:input", "Input A must be 1xN int32 vector.");
    if (!mxIsInt32(prhs[1]) || mxGetNumberOfDimensions(prhs[1]) != 2 || mxGetM(prhs[1]) != 1)
        mexErrMsgIdAndTxt("cmplx_mult_cmplx_q31:input", "Input B must be 1xN int32 vector.");

    mwSize nA = mxGetN(prhs[0]);
    mwSize nB = mxGetN(prhs[1]);

    if (nA != nB || nA % 2 != 0)
        mexErrMsgIdAndTxt("cmplx_mult_cmplx_q31:input", "Both inputs must be same length, even number of elements.");

    uint32_t numSamples = (uint32_t)(nA / 2);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)nA, mxINT32_CLASS, mxREAL);
    q31_t *srcA = (q31_t *)mxGetData(prhs[0]);
    q31_t *srcB = (q31_t *)mxGetData(prhs[1]);
    q31_t *dst  = (q31_t *)mxGetData(plhs[0]);

    arm_cmplx_mult_cmplx_q31(srcA, srcB, dst, numSamples);
}