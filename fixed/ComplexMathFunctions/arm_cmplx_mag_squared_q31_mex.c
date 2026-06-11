#include "mex.h"
#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_cmplx_mag_squared_q31(q31_t *pSrc, q31_t *pDst, uint32_t numSamples)
{
    q31_t real, imag;
    q31_t acc0, acc1;

    while (numSamples > 0u)
    {
        real = *pSrc++;
        imag = *pSrc++;
        acc0 = (q31_t)(((q63_t)real * (q63_t)real) >> 33);
        acc1 = (q31_t)(((q63_t)imag * (q63_t)imag) >> 33);
        *pDst++ = acc0 + acc1;
        numSamples--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 1) mexErrMsgIdAndTxt("cmplx_mag_squared_q31:input", "One input required.");
    if (nlhs > 1) mexErrMsgIdAndTxt("cmplx_mag_squared_q31:output", "One output required.");

    if (!mxIsInt32(prhs[0]) || mxGetNumberOfDimensions(prhs[0]) != 2 || mxGetM(prhs[0]) != 1)
        mexErrMsgIdAndTxt("cmplx_mag_squared_q31:input", "Input must be 1xN int32 vector.");

    mwSize numElements = mxGetN(prhs[0]);
    if (numElements % 2 != 0)
        mexErrMsgIdAndTxt("cmplx_mag_squared_q31:input", "Input length must be even.");

    uint32_t numSamples = (uint32_t)(numElements / 2);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)numSamples, mxINT32_CLASS, mxREAL);
    q31_t *src = (q31_t *)mxGetData(prhs[0]);
    q31_t *dst = (q31_t *)mxGetData(plhs[0]);

    arm_cmplx_mag_squared_q31(src, dst, numSamples);
}