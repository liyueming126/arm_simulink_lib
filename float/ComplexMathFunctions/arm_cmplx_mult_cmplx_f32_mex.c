#include "arm_cmplx_mult_cmplx_f32_simulink.c"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrcA, *pSrcB, *pDst;
    uint32_t numSamples;
    size_t N;

    if(nrhs != 2)
        mexErrMsgIdAndTxt("arm_cmplx_mult_cmplx_f32:InvalidInput", "Two inputs required (interleaved complex single)");
    if(nlhs > 1)
        mexErrMsgIdAndTxt("arm_cmplx_mult_cmplx_f32:InvalidOutput", "One output required");

    if(!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_cmplx_mult_cmplx_f32:InvalidType", "Input A must be real single array");
    if(!mxIsSingle(prhs[1]) || mxIsComplex(prhs[1]))
        mexErrMsgIdAndTxt("arm_cmplx_mult_cmplx_f32:InvalidType", "Input B must be real single array");

    N = mxGetNumberOfElements(prhs[0]);
    if(N != mxGetNumberOfElements(prhs[1]))
        mexErrMsgIdAndTxt("arm_cmplx_mult_cmplx_f32:InvalidSize", "Inputs must have same length");
    if(N % 2 != 0)
        mexErrMsgIdAndTxt("arm_cmplx_mult_cmplx_f32:InvalidSize", "Input length must be even (interleaved complex)");

    numSamples = (uint32_t)(N / 2);

    pSrcA = (float *)mxGetData(prhs[0]);
    pSrcB = (float *)mxGetData(prhs[1]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)N, mxSINGLE_CLASS, mxREAL);
    pDst = (float *)mxGetData(plhs[0]);

    arm_cmplx_mult_cmplx_f32(pSrcA, pSrcB, pDst, numSamples);
}