#include "mex.h"
#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_cmplx_dot_prod_q31(q31_t *pSrcA, q31_t *pSrcB, uint32_t numSamples, q63_t *realResult, q63_t *imagResult)
{
    q63_t real_sum = 0, imag_sum = 0;
    q31_t a0, b0, c0, d0;

    while (numSamples > 0u)
    {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += ((q63_t)a0 * (q63_t)c0) >> 14;
        imag_sum += ((q63_t)a0 * (q63_t)d0) >> 14;
        real_sum -= ((q63_t)b0 * (q63_t)d0) >> 14;
        imag_sum += ((q63_t)b0 * (q63_t)c0) >> 14;

        numSamples--;
    }

    *realResult = real_sum;
    *imagResult = imag_sum;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 2) mexErrMsgIdAndTxt("cmplx_dot_prod_q31:input", "Two inputs required.");
    if (nlhs != 2) mexErrMsgIdAndTxt("cmplx_dot_prod_q31:output", "Two outputs required.");

    if (!mxIsInt32(prhs[0]) || mxGetNumberOfDimensions(prhs[0]) != 2 || mxGetM(prhs[0]) != 1)
        mexErrMsgIdAndTxt("cmplx_dot_prod_q31:input", "Input A must be 1xN int32 vector.");
    if (!mxIsInt32(prhs[1]) || mxGetNumberOfDimensions(prhs[1]) != 2 || mxGetM(prhs[1]) != 1)
        mexErrMsgIdAndTxt("cmplx_dot_prod_q31:input", "Input B must be 1xN int32 vector.");

    mwSize nA = mxGetN(prhs[0]);
    mwSize nB = mxGetN(prhs[1]);

    if (nA != nB || nA % 2 != 0)
        mexErrMsgIdAndTxt("cmplx_dot_prod_q31:input", "Both inputs must be same length, even number of elements.");

    uint32_t numSamples = (uint32_t)(nA / 2);

    q63_t realResult, imagResult;
    q31_t *srcA = (q31_t *)mxGetData(prhs[0]);
    q31_t *srcB = (q31_t *)mxGetData(prhs[1]);

    arm_cmplx_dot_prod_q31(srcA, srcB, numSamples, &realResult, &imagResult);

    plhs[0] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
    plhs[1] = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
    *((q63_t *)mxGetData(plhs[0])) = realResult;
    *((q63_t *)mxGetData(plhs[1])) = imagResult;
}