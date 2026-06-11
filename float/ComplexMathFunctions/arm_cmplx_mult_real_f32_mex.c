#include "arm_cmplx_mult_real_f32_simulink.c"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrcCmplx, *pSrcReal, *pCmplxDst;
    uint32_t numSamples;
    size_t Ncmplx, Nreal;

    if(nrhs != 2)
        mexErrMsgIdAndTxt("arm_cmplx_mult_real_f32:InvalidInput", "Two inputs required (complex interleaved single, real single)");
    if(nlhs > 1)
        mexErrMsgIdAndTxt("arm_cmplx_mult_real_f32:InvalidOutput", "One output required");

    if(!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_cmplx_mult_real_f32:InvalidType", "Input A (complex) must be real single array");
    if(!mxIsSingle(prhs[1]) || mxIsComplex(prhs[1]))
        mexErrMsgIdAndTxt("arm_cmplx_mult_real_f32:InvalidType", "Input B (real) must be real single array");

    Ncmplx = mxGetNumberOfElements(prhs[0]);
    Nreal = mxGetNumberOfElements(prhs[1]);

    if(Ncmplx % 2 != 0)
        mexErrMsgIdAndTxt("arm_cmplx_mult_real_f32:InvalidSize", "Complex input length must be even (interleaved)");
    if(Ncmplx / 2 != Nreal)
        mexErrMsgIdAndTxt("arm_cmplx_mult_real_f32:InvalidSize", ...
            "Complex input length/2 must equal real input length");

    numSamples = (uint32_t)Nreal;

    pSrcCmplx = (float *)mxGetData(prhs[0]);
    pSrcReal = (float *)mxGetData(prhs[1]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)Ncmplx, mxSINGLE_CLASS, mxREAL);
    pCmplxDst = (float *)mxGetData(plhs[0]);

    arm_cmplx_mult_real_f32(pSrcCmplx, pSrcReal, pCmplxDst, numSamples);
}