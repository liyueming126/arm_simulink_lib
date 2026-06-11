#include "mex.h"
#include <stdint.h>

typedef int16_t q15_t;

static q15_t ssat_q15(int32_t val)
{
    if (val > 32767) return (q15_t)32767;
    if (val < -32768) return (q15_t)-32768;
    return (q15_t)val;
}

void arm_cmplx_mult_real_q15(q15_t *pSrcCmplx, q15_t *pSrcReal, q15_t *pCmplxDst, uint32_t numSamples)
{
    q15_t in;

    while (numSamples > 0u)
    {
        in = *pSrcReal++;
        *pCmplxDst++ = ssat_q15(((int32_t)(*pSrcCmplx++) * (int32_t)in) >> 15);
        *pCmplxDst++ = ssat_q15(((int32_t)(*pSrcCmplx++) * (int32_t)in) >> 15);
        numSamples--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 2) mexErrMsgIdAndTxt("cmplx_mult_real_q15:input", "Two inputs required.");
    if (nlhs > 1) mexErrMsgIdAndTxt("cmplx_mult_real_q15:output", "One output required.");

    if (!mxIsInt16(prhs[0]) || mxGetNumberOfDimensions(prhs[0]) != 2 || mxGetM(prhs[0]) != 1)
        mexErrMsgIdAndTxt("cmplx_mult_real_q15:input", "Complex input must be 1xN int16 vector.");
    if (!mxIsInt16(prhs[1]) || mxGetNumberOfDimensions(prhs[1]) != 2 || mxGetM(prhs[1]) != 1)
        mexErrMsgIdAndTxt("cmplx_mult_real_q15:input", "Real input must be 1xM int16 vector.");

    mwSize nCmplx = mxGetN(prhs[0]);
    mwSize nReal  = mxGetN(prhs[1]);

    if (nCmplx % 2 != 0)
        mexErrMsgIdAndTxt("cmplx_mult_real_q15:input", "Complex input length must be even.");
    if (nCmplx / 2 != nReal)
        mexErrMsgIdAndTxt("cmplx_mult_real_q15:input", "Complex samples must match real samples count.");

    uint32_t numSamples = (uint32_t)nReal;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)nCmplx, mxINT16_CLASS, mxREAL);
    q15_t *srcC = (q15_t *)mxGetData(prhs[0]);
    q15_t *srcR = (q15_t *)mxGetData(prhs[1]);
    q15_t *dst  = (q15_t *)mxGetData(plhs[0]);

    arm_cmplx_mult_real_q15(srcC, srcR, dst, numSamples);
}