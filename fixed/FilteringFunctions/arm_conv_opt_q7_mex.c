#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int8_t q7_t;
typedef int16_t q15_t;
typedef int32_t q31_t;

static void arm_fill_q15(q15_t value, q15_t *pDst, uint32_t blockSize)
{
    uint32_t blkCnt = blockSize;
    while (blkCnt > 0u)
    {
        *pDst++ = value;
        blkCnt--;
    }
}

static void arm_conv_opt_q7(
    q7_t *pSrcA,
    uint32_t srcALen,
    q7_t *pSrcB,
    uint32_t srcBLen,
    q7_t *pDst,
    q15_t *pScratch1,
    q15_t *pScratch2)
{
    q15_t *pScr2 = pScratch2;
    q15_t *pScr1 = pScratch1;
    q7_t *pIn1, *pIn2;
    uint32_t j, k, blkCnt, tapCnt;
    q7_t *px;
    q15_t *py;
    q31_t acc0;
    q7_t *pOut = pDst;

    if (srcALen >= srcBLen)
    {
        pIn1 = pSrcA;
        pIn2 = pSrcB;
    }
    else
    {
        pIn1 = pSrcB;
        pIn2 = pSrcA;
        j = srcBLen;
        srcBLen = srcALen;
        srcALen = j;
    }

    px = pIn2 + srcBLen - 1;
    k = srcBLen;
    while (k > 0u)
    {
        *pScr2++ = (q15_t)*px--;
        k--;
    }

    arm_fill_q15(0, pScr1, (srcBLen - 1u));
    pScr1 += (srcBLen - 1u);

    k = srcALen;
    while (k > 0u)
    {
        *pScr1++ = (q15_t)*pIn1++;
        k--;
    }

    arm_fill_q15(0, pScr1, (srcBLen - 1u));
    pScr1 += (srcBLen - 1u);

    py = pScratch2;
    pScr2 = py;

    blkCnt = (srcALen + srcBLen - 1u);

    while (blkCnt > 0u)
    {
        pScr1 = pScratch1;
        acc0 = 0;
        tapCnt = srcBLen;

        while (tapCnt > 0u)
        {
            acc0 += (q31_t)(*pScr1++) * (q31_t)(*pScr2++);
            tapCnt--;
        }

        acc0 = acc0 >> 7;
        if (acc0 > 127) acc0 = 127;
        if (acc0 < -128) acc0 = -128;
        *pOut++ = (q7_t)acc0;

        pScr2 = py;
        pScratch1 += 1u;
        blkCnt--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrcA, *pSrcB, *pDst;
    int16_t *pScratch1, *pScratch2;
    size_t srcALen, srcBLen;
    size_t scratch1Size, scratch2Size;

    if (nrhs != 2)
    {
        mexErrMsgIdAndTxt("arm_conv_opt_q7_mex:invalidInput",
            "Two inputs required: signalA (int8), signalB (int8).");
    }

    if (nlhs > 1)
    {
        mexErrMsgIdAndTxt("arm_conv_opt_q7_mex:invalidOutput",
            "At most one output argument allowed.");
    }

    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]))
    {
        mexErrMsgIdAndTxt("arm_conv_opt_q7_mex:invalidInputType",
            "First input must be a real int8 array.");
    }

    if (!mxIsInt8(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_conv_opt_q7_mex:invalidInputType",
            "Second input must be a real int8 array.");
    }

    pSrcA = (int8_t *)mxGetData(prhs[0]);
    srcALen = mxGetNumberOfElements(prhs[0]);

    pSrcB = (int8_t *)mxGetData(prhs[1]);
    srcBLen = mxGetNumberOfElements(prhs[1]);

    if (srcALen >= srcBLen)
    {
        scratch1Size = srcALen + 2 * srcBLen - 2;
        scratch2Size = srcBLen;
    }
    else
    {
        scratch1Size = srcBLen + 2 * srcALen - 2;
        scratch2Size = srcALen;
    }

    pScratch1 = (int16_t *)mxCalloc(scratch1Size, sizeof(int16_t));
    pScratch2 = (int16_t *)mxCalloc(scratch2Size, sizeof(int16_t));

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(srcALen + srcBLen - 1), mxINT8_CLASS, mxREAL);
    pDst = (int8_t *)mxGetData(plhs[0]);

    arm_conv_opt_q7(pSrcA, (uint32_t)srcALen, pSrcB, (uint32_t)srcBLen, pDst, pScratch1, pScratch2);

    mxFree(pScratch1);
    mxFree(pScratch2);
}