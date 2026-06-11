/* MEX wrapper for arm_conv_partial_f32 */
#include "mex.h"
#include <string.h>

#include "arm_conv_partial_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float32_t *pSrcA, *pSrcB, *pDst;
    uint32_t srcALen, srcBLen, firstIndex, numPoints;
    arm_status status;

    if (nrhs != 4) {
        mexErrMsgIdAndTxt("arm_conv_partial_f32:InvalidInput", "Four inputs: signal A, signal B, firstIndex, numPoints");
    }
    if (nlhs > 2) {
        mexErrMsgIdAndTxt("arm_conv_partial_f32:InvalidOutput", "Two outputs: result, status");
    }
    if (!mxIsSingle(prhs[0]) || !mxIsSingle(prhs[1])) {
        mexErrMsgIdAndTxt("arm_conv_partial_f32:NotSingle", "Signal inputs must be single precision");
    }

    pSrcA = (float32_t *)mxGetData(prhs[0]);
    pSrcB = (float32_t *)mxGetData(prhs[1]);
    srcALen = (uint32_t)mxGetNumberOfElements(prhs[0]);
    srcBLen = (uint32_t)mxGetNumberOfElements(prhs[1]);
    firstIndex = (uint32_t)mxGetScalar(prhs[2]);
    numPoints = (uint32_t)mxGetScalar(prhs[3]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(srcALen + srcBLen - 1), mxSINGLE_CLASS, mxREAL);
    pDst = (float32_t *)mxGetData(plhs[0]);
    /* Initialize to zero */
    {
        mwSize k;
        for (k = 0; k < (mwSize)(srcALen + srcBLen - 1); k++) pDst[k] = 0.0f;
    }

    status = arm_conv_partial_f32(pSrcA, srcALen, pSrcB, srcBLen, pDst, firstIndex, numPoints);

    plhs[1] = mxCreateDoubleScalar((double)status);
}