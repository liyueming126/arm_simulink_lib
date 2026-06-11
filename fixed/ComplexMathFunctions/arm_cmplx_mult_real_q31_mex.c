#include "mex.h"
#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

static q31_t clip_q63_to_q31(q63_t x)
{
    return ((q31_t)(x >> 32) != ((q31_t)x >> 31))
        ? (q31_t)(0x7FFFFFFF ^ ((q31_t)(x >> 63)))
        : (q31_t)x;
}

void arm_cmplx_mult_real_q31(q31_t *pSrcCmplx, q31_t *pSrcReal, q31_t *pCmplxDst, uint32_t numSamples)
{
    q31_t inA1;

    while (numSamples > 0u)
    {
        inA1 = *pSrcReal++;
        *pCmplxDst++ = (q31_t)clip_q63_to_q31(((q63_t)(*pSrcCmplx++) * (q63_t)inA1) >> 31);
        *pCmplxDst++ = (q31_t)clip_q63_to_q31(((q63_t)(*pSrcCmplx++) * (q63_t)inA1) >> 31);
        numSamples--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 2) mexErrMsgIdAndTxt("cmplx_mult_real_q31:input", "Two inputs required.");
    if (nlhs > 1) mexErrMsgIdAndTxt("cmplx_mult_real_q31:output", "One output required.");

    if (!mxIsInt32(prhs[0]) || mxGetNumberOfDimensions(prhs[0]) != 2 || mxGetM(prhs[0]) != 1)
        mexErrMsgIdAndTxt("cmplx_mult_real_q31:input", "Complex input must be 1xN int32 vector.");
    if (!mxIsInt32(prhs[1]) || mxGetNumberOfDimensions(prhs[1]) != 2 || mxGetM(prhs[1]) != 1)
        mexErrMsgIdAndTxt("cmplx_mult_real_q31:input", "Real input must be 1xM int32 vector.");

    mwSize nCmplx = mxGetN(prhs[0]);
    mwSize nReal  = mxGetN(prhs[1]);

    if (nCmplx % 2 != 0)
        mexErrMsgIdAndTxt("cmplx_mult_real_q31:input", "Complex input length must be even.");
    if (nCmplx / 2 != nReal)
        mexErrMsgIdAndTxt("cmplx_mult_real_q31:input", "Complex samples must match real samples count.");

    uint32_t numSamples = (uint32_t)nReal;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)nCmplx, mxINT32_CLASS, mxREAL);
    q31_t *srcC = (q31_t *)mxGetData(prhs[0]);
    q31_t *srcR = (q31_t *)mxGetData(prhs[1]);
    q31_t *dst  = (q31_t *)mxGetData(plhs[0]);

    arm_cmplx_mult_real_q31(srcC, srcR, dst, numSamples);
}