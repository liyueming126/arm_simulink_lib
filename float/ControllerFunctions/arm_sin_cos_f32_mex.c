#include <stdint.h>
#include <math.h>
#include "mex.h"

void arm_sin_cos_f32(
    float theta,
    float * pSinVal,
    float * pCosVal)
{
    *pSinVal = sinf(theta * 3.14159265358979f / 180.0f);
    *pCosVal = cosf(theta * 3.14159265358979f / 180.0f);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float theta, sinVal, cosVal;
    float *pOut;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_sin_cos_f32:invalidInput", "1 input required: theta (scalar single, degrees).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_sin_cos_f32:invalidOutput", "At most 1 output allowed.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]) || mxGetNumberOfElements(prhs[0]) != 1)
        mexErrMsgIdAndTxt("arm_sin_cos_f32:invalidInputType", "theta must be scalar single.");

    theta = *(float *)mxGetData(prhs[0]);
    arm_sin_cos_f32(theta, &sinVal, &cosVal);

    plhs[0] = mxCreateNumericMatrix(1, 2, mxSINGLE_CLASS, mxREAL);
    pOut = (float *)mxGetData(plhs[0]);
    pOut[0] = sinVal;
    pOut[1] = cosVal;
}