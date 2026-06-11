#include <stdint.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;
typedef int32_t arm_status;
#define ARM_MATH_SUCCESS 0
#define ARM_MATH_ARGUMENT_ERROR 1

static arm_status arm_conv_partial_q15(
    q15_t *pSrcA,
    uint32_t srcALen,
    q15_t *pSrcB,
    uint32_t srcBLen,
    q15_t *pDst,
    uint32_t firstIndex,
    uint32_t numPoints)
{
    q63_t sum;
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
                sum += (q31_t)pSrcA[j] * pSrcB[i - j];
            }
        }

        if (sum > 0)
        {
            sum = (sum >> 15u);
            pDst[i] = (q15_t)(sum > 32767 ? 32767 : sum);
        }
        else
        {
            sum = (sum >> 15u);
            pDst[i] = (q15_t)(sum < -32768 ? -32768 : sum);
        }
    }

    return ARM_MATH_SUCCESS;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrcA, *pSrcB, *pDst;
    size_t srcALen, srcBLen, firstIndex, numPoints;
    arm_status status;
    size_t totalLen;

    if (nrhs != 4)
    {
        mexErrMsgIdAndTxt("arm_conv_partial_q15_mex:invalidInput",
            "Four inputs: signalA (int16), signalB (int16), firstIndex (scalar), numPoints (scalar).");
    }

    if (nlhs > 1)
    {
        mexErrMsgIdAndTxt("arm_conv_partial_q15_mex:invalidOutput",
            "At most one output.");
    }

    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]))
    {
        mexErrMsgIdAndTxt("arm_conv_partial_q15_mex:invalidInputType",
            "First input must be a real int16 array.");
    }

    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_conv_partial_q15_mex:invalidInputType",
            "Second input must be a real int16 array.");
    }

    pSrcA = (int16_t *)mxGetData(prhs[0]);
    srcALen = mxGetNumberOfElements(prhs[0]);
    pSrcB = (int16_t *)mxGetData(prhs[1]);
    srcBLen = mxGetNumberOfElements(prhs[1]);

    firstIndex = (size_t)mxGetScalar(prhs[2]);
    numPoints = (size_t)mxGetScalar(prhs[3]);

    totalLen = srcALen + srcBLen - 1;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)totalLen, mxINT16_CLASS, mxREAL);
    pDst = (int16_t *)mxGetData(plhs[0]);
    memset(pDst, 0, totalLen * sizeof(int16_t));

    status = arm_conv_partial_q15(pSrcA, (uint32_t)srcALen, pSrcB, (uint32_t)srcBLen,
        pDst, (uint32_t)firstIndex, (uint32_t)numPoints);

    if (status == ARM_MATH_ARGUMENT_ERROR)
    {
        mexErrMsgIdAndTxt("arm_conv_partial_q15_mex:argError",
            "Requested subset out of range.");
    }
}