#include "arm_min_f32_simulink.c"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, minVal;
    uint32_t blockSize, index;
    uint32_t *pIndex = &index;

    if(nrhs != 1) mexErrMsgTxt("One input required.");
    if(nlhs > 2) mexErrMsgTxt("Up to two outputs allowed.");
    if(!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0])) mexErrMsgTxt("Input must be real single.");

    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);
    pSrc = (float *)mxGetData(prhs[0]);

    arm_min_f32(pSrc, blockSize, &minVal, pIndex);

    plhs[0] = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
    *(float *)mxGetData(plhs[0]) = minVal;

    if(nlhs >= 2)
    {
        plhs[1] = mxCreateDoubleScalar((double)(index + 1)); /* 1-based index */
    }
}