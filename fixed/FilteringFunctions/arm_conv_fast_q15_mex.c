#include <stdint.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;

static void arm_conv_fast_q15(
    q15_t *pSrcA,
    uint32_t srcALen,
    q15_t *pSrcB,
    uint32_t srcBLen,
    q15_t *pDst)
{
    q31_t sum;
    uint32_t i, j;

    for (i = 0; i < (srcALen + srcBLen - 1); i++)
    {
        sum = 0;

        for (j = 0; j <= i; j++)
        {
            if (((i - j) < srcBLen) && (j < srcALen))
            {
                sum += (q31_t)pSrcA[j] * pSrcB[i - j];
            }
        }

        pDst[i] = (q15_t)(sum >> 15);
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrcA, *pSrcB, *pDst;
    size_t srcALen, srcBLen;

    if (nrhs != 2)
    {
        mexErrMsgIdAndTxt("arm_conv_fast_q15_mex:invalidInput",
            "Two inputs required: signalA (int16), signalB (int16).");
    }

    if (nlhs > 1)
    {
        mexErrMsgIdAndTxt("arm_conv_fast_q15_mex:invalidOutput",
            "At most one output argument allowed.");
    }

    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]))
    {
        mexErrMsgIdAndTxt("arm_conv_fast_q15_mex:invalidInputType",
            "First input must be a real int16 array.");
    }

    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_conv_fast_q15_mex:invalidInputType",
            "Second input must be a real int16 array.");
    }

    pSrcA = (int16_t *)mxGetData(prhs[0]);
    srcALen = mxGetNumberOfElements(prhs[0]);

    pSrcB = (int16_t *)mxGetData(prhs[1]);
    srcBLen = mxGetNumberOfElements(prhs[1]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(srcALen + srcBLen - 1), mxINT16_CLASS, mxREAL);
    pDst = (int16_t *)mxGetData(plhs[0]);

    arm_conv_fast_q15(pSrcA, (uint32_t)srcALen, pSrcB, (uint32_t)srcBLen, pDst);
}