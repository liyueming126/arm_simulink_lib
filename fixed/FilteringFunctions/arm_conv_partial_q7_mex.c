#include <stdint.h>
#include "mex.h"

typedef int8_t q7_t;
typedef int32_t arm_status;
#define ARM_MATH_SUCCESS 0
#define ARM_MATH_ARGUMENT_ERROR 1

static arm_status arm_conv_partial_q7(
    q7_t *pSrcA,
    uint32_t srcALen,
    q7_t *pSrcB,
    uint32_t srcBLen,
    q7_t *pDst,
    uint32_t firstIndex,
    uint32_t numPoints)
{
    int32_t sum;
    uint32_t i, j;
    arm_status status;

    if ((firstIndex + numPoints) > (srcALen + (srcBLen - 1u)))
    {
        status = ARM_MATH_ARGUMENT_ERROR;
    }
    else
    {
        for (i = firstIndex; i < (firstIndex + numPoints); i++)
        {
            sum = 0;

            for (j = 0; j <= i; j++)
            {
                if (((i - j) < srcBLen) && (j < srcALen))
                {
                    sum += (int16_t)pSrcA[j] * (int16_t)pSrcB[i - j];
                }
            }

            sum = sum >> 7;
            if (sum > 127) sum = 127;
            if (sum < -128) sum = -128;
            *pDst++ = (q7_t)sum;
        }
        status = ARM_MATH_SUCCESS;
    }

    return status;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrcA, *pSrcB, *pDst;
    size_t srcALen, srcBLen;
    uint32_t firstIndex, numPoints;
    arm_status status;

    if (nrhs != 4)
    {
        mexErrMsgIdAndTxt("arm_conv_partial_q7_mex:invalidInput",
            "Four inputs required: signalA (int8), signalB (int8), firstIndex, numPoints.");
    }

    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]))
    {
        mexErrMsgIdAndTxt("arm_conv_partial_q7_mex:invalidInputType",
            "First input must be a real int8 array.");
    }

    if (!mxIsInt8(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_conv_partial_q7_mex:invalidInputType",
            "Second input must be a real int8 array.");
    }

    pSrcA = (int8_t *)mxGetData(prhs[0]);
    srcALen = mxGetNumberOfElements(prhs[0]);

    pSrcB = (int8_t *)mxGetData(prhs[1]);
    srcBLen = mxGetNumberOfElements(prhs[1]);

    firstIndex = (uint32_t)mxGetScalar(prhs[2]);
    numPoints = (uint32_t)mxGetScalar(prhs[3]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)numPoints, mxINT8_CLASS, mxREAL);
    pDst = (int8_t *)mxGetData(plhs[0]);

    status = arm_conv_partial_q7(pSrcA, (uint32_t)srcALen, pSrcB, (uint32_t)srcBLen, pDst, firstIndex, numPoints);

    if (status != ARM_MATH_SUCCESS)
    {
        mexErrMsgIdAndTxt("arm_conv_partial_q7_mex:argError",
            "Requested range [%d, %d] is out of bounds [0, %d].",
            firstIndex, firstIndex + numPoints, srcALen + srcBLen - 1);
    }
}