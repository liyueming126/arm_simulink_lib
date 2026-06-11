#include <stdint.h>
#include "mex.h"

typedef int8_t q7_t;

static void arm_correlate_q7(
    q7_t *pSrcA,
    uint32_t srcALen,
    q7_t *pSrcB,
    uint32_t srcBLen,
    q7_t *pDst)
{
    int32_t sum;
    int32_t k, n;
    int32_t idxB;
    int32_t sALen = (int32_t)srcALen;
    int32_t sBLen = (int32_t)srcBLen;
    int32_t total = sALen + sBLen - 1;

    for (k = 0; k < total; k++)
    {
        sum = 0;

        for (n = 0; n < sALen; n++)
        {
            idxB = n - k + sBLen - 1;
            if (idxB >= 0 && idxB < sBLen)
            {
                sum += (int16_t)pSrcA[n] * (int16_t)pSrcB[idxB];
            }
        }

        sum = sum >> 7;
        if (sum > 127) sum = 127;
        if (sum < -128) sum = -128;
        pDst[k] = (q7_t)sum;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrcA, *pSrcB, *pDst;
    size_t srcALen, srcBLen;

    if (nrhs != 2)
    {
        mexErrMsgIdAndTxt("arm_correlate_q7_mex:invalidInput",
            "Two inputs required: signalA (int8), signalB (int8).");
    }

    if (nlhs > 1)
    {
        mexErrMsgIdAndTxt("arm_correlate_q7_mex:invalidOutput",
            "At most one output argument allowed.");
    }

    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]))
    {
        mexErrMsgIdAndTxt("arm_correlate_q7_mex:invalidInputType",
            "First input must be a real int8 array.");
    }

    if (!mxIsInt8(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_correlate_q7_mex:invalidInputType",
            "Second input must be a real int8 array.");
    }

    pSrcA = (int8_t *)mxGetData(prhs[0]);
    srcALen = mxGetNumberOfElements(prhs[0]);
    pSrcB = (int8_t *)mxGetData(prhs[1]);
    srcBLen = mxGetNumberOfElements(prhs[1]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(srcALen + srcBLen - 1), mxINT8_CLASS, mxREAL);
    pDst = (int8_t *)mxGetData(plhs[0]);

    arm_correlate_q7(pSrcA, (uint32_t)srcALen, pSrcB, (uint32_t)srcBLen, pDst);
}