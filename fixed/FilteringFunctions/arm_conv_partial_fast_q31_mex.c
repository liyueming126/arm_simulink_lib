#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int64_t q63_t;
typedef int32_t arm_status;
#define ARM_MATH_SUCCESS 0
#define ARM_MATH_ARGUMENT_ERROR 1

static arm_status arm_conv_partial_fast_q31(
    q31_t *pSrcA,
    uint32_t srcALen,
    q31_t *pSrcB,
    uint32_t srcBLen,
    q31_t *pDst,
    uint32_t firstIndex,
    uint32_t numPoints)
{
    q31_t sum;
    uint32_t i, j;

    if ((firstIndex + numPoints) > (srcALen + srcBLen - 1u))
    {
        return ARM_MATH_ARGUMENT_ERROR;
    }

    for (i = firstIndex; i < firstIndex + numPoints; i++)
    {
        sum = 0;

        for (j = 0; j <= i; j++)
        {
            if (((i - j) < srcBLen) && (j < srcALen))
            {
                sum = (q31_t)((((q63_t)sum << 32) + ((q63_t)pSrcA[j] * pSrcB[i - j])) >> 32);
            }
        }

        pDst[i] = (q31_t)(sum << 1);
    }

    return ARM_MATH_SUCCESS;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrcA, *pSrcB, *pDst;
    size_t srcALen, srcBLen, firstIndex, numPoints;
    size_t totalLen;
    arm_status status;

    if (nrhs != 4)
    {
        mexErrMsgIdAndTxt("arm_conv_partial_fast_q31_mex:invalidInput",
            "Four inputs: signalA (int32), signalB (int32), firstIndex, numPoints.");
    }

    if (nlhs > 1)
    {
        mexErrMsgIdAndTxt("arm_conv_partial_fast_q31_mex:invalidOutput",
            "At most one output.");
    }

    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]))
    {
        mexErrMsgIdAndTxt("arm_conv_partial_fast_q31_mex:invalidInputType",
            "First input must be a real int32 array.");
    }

    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_conv_partial_fast_q31_mex:invalidInputType",
            "Second input must be a real int32 array.");
    }

    pSrcA = (int32_t *)mxGetData(prhs[0]);
    srcALen = mxGetNumberOfElements(prhs[0]);
    pSrcB = (int32_t *)mxGetData(prhs[1]);
    srcBLen = mxGetNumberOfElements(prhs[1]);

    firstIndex = (size_t)mxGetScalar(prhs[2]);
    numPoints = (size_t)mxGetScalar(prhs[3]);

    totalLen = srcALen + srcBLen - 1;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)totalLen, mxINT32_CLASS, mxREAL);
    pDst = (int32_t *)mxGetData(plhs[0]);
    memset(pDst, 0, totalLen * sizeof(int32_t));

    status = arm_conv_partial_fast_q31(pSrcA, (uint32_t)srcALen, pSrcB, (uint32_t)srcBLen,
        pDst, (uint32_t)firstIndex, (uint32_t)numPoints);

    if (status == ARM_MATH_ARGUMENT_ERROR)
    {
        mexErrMsgIdAndTxt("arm_conv_partial_fast_q31_mex:argError",
            "Requested subset out of range.");
    }
}