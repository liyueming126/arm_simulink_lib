#include "mex.h"
#include <stdint.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_cmplx_dot_prod_q15(q15_t *pSrcA, q15_t *pSrcB, uint32_t numSamples, q31_t *realResult, q31_t *imagResult)
{
    q63_t real_sum = 0, imag_sum = 0;
    q15_t a0, b0, c0, d0;

    while (numSamples > 0u)
    {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += (q31_t)a0 * (q31_t)c0;
        imag_sum += (q31_t)a0 * (q31_t)d0;
        real_sum -= (q31_t)b0 * (q31_t)d0;
        imag_sum += (q31_t)b0 * (q31_t)c0;

        numSamples--;
    }

    *realResult = (q31_t)(real_sum >> 6);
    *imagResult = (q31_t)(imag_sum >> 6);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 2) mexErrMsgIdAndTxt("cmplx_dot_prod_q15:input", "Two inputs required.");
    if (nlhs != 2) mexErrMsgIdAndTxt("cmplx_dot_prod_q15:output", "Two outputs required.");

    if (!mxIsInt16(prhs[0]) || mxGetNumberOfDimensions(prhs[0]) != 2 || mxGetM(prhs[0]) != 1)
        mexErrMsgIdAndTxt("cmplx_dot_prod_q15:input", "Input A must be 1xN int16 vector.");
    if (!mxIsInt16(prhs[1]) || mxGetNumberOfDimensions(prhs[1]) != 2 || mxGetM(prhs[1]) != 1)
        mexErrMsgIdAndTxt("cmplx_dot_prod_q15:input", "Input B must be 1xN int16 vector.");

    mwSize nA = mxGetN(prhs[0]);
    mwSize nB = mxGetN(prhs[1]);

    if (nA != nB || nA % 2 != 0)
        mexErrMsgIdAndTxt("cmplx_dot_prod_q15:input", "Both inputs must be same length, even number of elements.");

    uint32_t numSamples = (uint32_t)(nA / 2);

    q31_t realResult, imagResult;
    q15_t *srcA = (q15_t *)mxGetData(prhs[0]);
    q15_t *srcB = (q15_t *)mxGetData(prhs[1]);

    arm_cmplx_dot_prod_q15(srcA, srcB, numSamples, &realResult, &imagResult);

    plhs[0] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    plhs[1] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    *((q31_t *)mxGetData(plhs[0])) = realResult;
    *((q31_t *)mxGetData(plhs[1])) = imagResult;
}