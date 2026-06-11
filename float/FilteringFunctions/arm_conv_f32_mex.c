/* MEX wrapper for arm_conv_f32 */
#include "mex.h"
#include <string.h>

#include "arm_conv_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float32_t *pSrcA, *pSrcB, *pDst;
    uint32_t srcALen, srcBLen;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("arm_conv_f32:InvalidInput", "Two inputs required: signal A, signal B");
    }
    if (nlhs > 1) {
        mexErrMsgIdAndTxt("arm_conv_f32:InvalidOutput", "One output required");
    }
    if (!mxIsSingle(prhs[0]) || !mxIsSingle(prhs[1])) {
        mexErrMsgIdAndTxt("arm_conv_f32:NotSingle", "Inputs must be single precision");
    }

    pSrcA = (float32_t *)mxGetData(prhs[0]);
    pSrcB = (float32_t *)mxGetData(prhs[1]);
    srcALen = (uint32_t)mxGetNumberOfElements(prhs[0]);
    srcBLen = (uint32_t)mxGetNumberOfElements(prhs[1]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(srcALen + srcBLen - 1), mxSINGLE_CLASS, mxREAL);
    pDst = (float32_t *)mxGetData(plhs[0]);

    arm_conv_f32(pSrcA, srcALen, pSrcB, srcBLen, pDst);
}