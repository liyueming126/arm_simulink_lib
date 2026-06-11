#include "arm_cmplx_dot_prod_f32_simulink.c"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrcA, *pSrcB;
    float32_t realResult, imagResult;
    uint32_t numSamples;
    size_t N;

    if(nrhs != 2)
        mexErrMsgIdAndTxt("arm_cmplx_dot_prod_f32:InvalidInput", "Two inputs required (interleaved complex single)");
    if(nlhs > 2)
        mexErrMsgIdAndTxt("arm_cmplx_dot_prod_f32:InvalidOutput", "Up to two outputs accepted");

    if(!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_cmplx_dot_prod_f32:InvalidType", "Input A must be real single array");
    if(!mxIsSingle(prhs[1]) || mxIsComplex(prhs[1]))
        mexErrMsgIdAndTxt("arm_cmplx_dot_prod_f32:InvalidType", "Input B must be real single array");

    N = mxGetNumberOfElements(prhs[0]);
    if(N != mxGetNumberOfElements(prhs[1]))
        mexErrMsgIdAndTxt("arm_cmplx_dot_prod_f32:InvalidSize", "Inputs must have same length");
    if(N % 2 != 0)
        mexErrMsgIdAndTxt("arm_cmplx_dot_prod_f32:InvalidSize", "Input length must be even (interleaved complex)");

    numSamples = (uint32_t)(N / 2);

    pSrcA = (float *)mxGetData(prhs[0]);
    pSrcB = (float *)mxGetData(prhs[1]);

    arm_cmplx_dot_prod_f32(pSrcA, pSrcB, numSamples, &realResult, &imagResult);

    plhs[0] = mxCreateDoubleScalar((double)realResult);
    plhs[1] = mxCreateDoubleScalar((double)imagResult);
}