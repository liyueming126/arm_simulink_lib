#include "mex.h"
#include <stdint.h>

typedef int16_t q15_t;
typedef int64_t q63_t;

void arm_cmplx_mag_squared_q15(q15_t *pSrc, q15_t *pDst, uint32_t numSamples)
{
    q15_t real, imag;
    int32_t acc0, acc1;

    while (numSamples > 0u)
    {
        real = *pSrc++;
        imag = *pSrc++;
        acc0 = (int32_t)real * (int32_t)real;
        acc1 = (int32_t)imag * (int32_t)imag;
        *pDst++ = (q15_t)(((q63_t)acc0 + (q63_t)acc1) >> 17);
        numSamples--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 1) mexErrMsgIdAndTxt("cmplx_mag_squared_q15:input", "One input required.");
    if (nlhs > 1) mexErrMsgIdAndTxt("cmplx_mag_squared_q15:output", "One output required.");

    if (!mxIsInt16(prhs[0]) || mxGetNumberOfDimensions(prhs[0]) != 2 || mxGetM(prhs[0]) != 1)
        mexErrMsgIdAndTxt("cmplx_mag_squared_q15:input", "Input must be 1xN int16 vector.");

    mwSize numElements = mxGetN(prhs[0]);
    if (numElements % 2 != 0)
        mexErrMsgIdAndTxt("cmplx_mag_squared_q15:input", "Input length must be even.");

    uint32_t numSamples = (uint32_t)(numElements / 2);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)numSamples, mxINT16_CLASS, mxREAL);
    q15_t *src = (q15_t *)mxGetData(prhs[0]);
    q15_t *dst = (q15_t *)mxGetData(plhs[0]);

    arm_cmplx_mag_squared_q15(src, dst, numSamples);
}