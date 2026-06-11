#include "arm_power_f32_simulink.c"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, result;
    uint32_t blockSize;

    if(nrhs != 1) mexErrMsgTxt("One input required.");
    if(nlhs > 1) mexErrMsgTxt("One output required.");
    if(!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0])) mexErrMsgTxt("Input must be real single.");

    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);
    pSrc = (float *)mxGetData(prhs[0]);

    arm_power_f32(pSrc, blockSize, &result);

    plhs[0] = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
    *(float *)mxGetData(plhs[0]) = result;
}