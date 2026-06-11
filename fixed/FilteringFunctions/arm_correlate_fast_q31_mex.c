#include <stdint.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int64_t q63_t;

static void arm_correlate_fast_q31(
    q31_t *pSrcA,
    uint32_t srcALen,
    q31_t *pSrcB,
    uint32_t srcBLen,
    q31_t *pDst)
{
    q31_t sum;
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
                sum = (q31_t)((((q63_t)sum << 32) + ((q63_t)pSrcA[n] * pSrcB[idxB])) >> 32);
            }
        }

        pDst[k] = (q31_t)(sum << 1);
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrcA, *pSrcB, *pDst;
    size_t srcALen, srcBLen;

    if (nrhs != 2)
    {
        mexErrMsgIdAndTxt("arm_correlate_fast_q31_mex:invalidInput",
            "Two inputs required: int32 arrays signalA and signalB.");
    }

    if (nlhs > 1)
    {
        mexErrMsgIdAndTxt("arm_correlate_fast_q31_mex:invalidOutput",
            "At most one output argument allowed.");
    }

    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]))
    {
        mexErrMsgIdAndTxt("arm_correlate_fast_q31_mex:invalidInputType",
            "First input must be a real int32 array.");
    }

    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_correlate_fast_q31_mex:invalidInputType",
            "Second input must be a real int32 array.");
    }

    pSrcA = (int32_t *)mxGetData(prhs[0]);
    srcALen = mxGetNumberOfElements(prhs[0]);
    pSrcB = (int32_t *)mxGetData(prhs[1]);
    srcBLen = mxGetNumberOfElements(prhs[1]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(srcALen + srcBLen - 1), mxINT32_CLASS, mxREAL);
    pDst = (int32_t *)mxGetData(plhs[0]);

    arm_correlate_fast_q31(pSrcA, (uint32_t)srcALen, pSrcB, (uint32_t)srcBLen, pDst);
}