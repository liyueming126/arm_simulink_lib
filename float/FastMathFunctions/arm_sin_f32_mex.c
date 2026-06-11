#include <stdint.h>
#include <math.h>
#include "mex.h"

float arm_sin_f32(
    float x)
{
    return sinf(x);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float x, y;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_sin_f32:invalidInput", "1 input required: x (scalar single, radians).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_sin_f32:invalidOutput", "At most 1 output allowed.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]) || mxGetNumberOfElements(prhs[0]) != 1)
        mexErrMsgIdAndTxt("arm_sin_f32:invalidInputType", "x must be scalar single.");

    x = *(float *)mxGetData(prhs[0]);
    y = arm_sin_f32(x);

    plhs[0] = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
    *(float *)mxGetData(plhs[0]) = y;
}