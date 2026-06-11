#include "arm_cmplx_conj_f32_simulink.c"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, *pDst;
    uint32_t numSamples;
    size_t N;

    if(nrhs != 1)
        mexErrMsgIdAndTxt("arm_cmplx_conj_f32:InvalidInput", "One input required (interleaved complex single)");
    if(nlhs > 1)
        mexErrMsgIdAndTxt("arm_cmplx_conj_f32:InvalidOutput", "One output required");

    if(!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_cmplx_conj_f32:InvalidType", "Input must be real single array");

    N = mxGetNumberOfElements(prhs[0]);
    if(N % 2 != 0)
        mexErrMsgIdAndTxt("arm_cmplx_conj_f32:InvalidSize", "Input length must be even (interleaved complex)");

    numSamples = (uint32_t)(N / 2);

    pSrc = (float *)mxGetData(prhs[0]);
    plhs[0] = mxCreateNumericMatrix(1, (mwSize)N, mxSINGLE_CLASS, mxREAL);
    pDst = (float *)mxGetData(plhs[0]);

    arm_cmplx_conj_f32(pSrc, pDst, numSamples);
}