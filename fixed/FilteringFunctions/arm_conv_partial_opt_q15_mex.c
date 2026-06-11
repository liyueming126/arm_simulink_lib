#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;
typedef int32_t arm_status;

static int16_t ssat16(int32_t val)
{
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (int16_t)val;
}

static arm_status arm_conv_partial_opt_q15(
    q15_t *pSrcA, uint32_t srcALen,
    q15_t *pSrcB, uint32_t srcBLen,
    q15_t *pDst,
    uint32_t firstIndex, uint32_t numPoints,
    q15_t *pScratch1, q15_t *pScratch2)
{
    q15_t *pIn1, *pIn2, *px, *py, *pScr1, *pOut;
    uint32_t j, k, blkCnt, tapCnt;
    q63_t acc0;

    if ((firstIndex + numPoints) > (srcALen + srcBLen - 1u)) return 1;

    if (srcALen >= srcBLen) { pIn1 = pSrcA; pIn2 = pSrcB; }
    else { pIn1 = pSrcB; pIn2 = pSrcA; j = srcBLen; srcBLen = srcALen; srcALen = j; }

    pScr1 = pScratch2 + srcBLen - 1;
    px = pIn2;
    k = srcBLen; while (k > 0u) { *pScr1-- = *px++; k--; }

    pScr1 = pScratch1;
    for (k = 0; k < srcBLen - 1u; k++) pScr1[k] = 0;
    pScr1 += (srcBLen - 1u);
    for (k = 0; k < srcALen; k++) pScr1[k] = pIn1[k];
    pScr1 += srcALen;
    for (k = 0; k < srcBLen - 1u; k++) pScr1[k] = 0;

    py = pScratch2;
    pOut = pDst;

    for (blkCnt = 0; blkCnt < numPoints; blkCnt++)
    {
        pScr1 = pScratch1 + firstIndex + blkCnt;
        acc0 = 0; tapCnt = srcBLen;
        while (tapCnt > 0u) { acc0 += (q63_t)(*pScr1++) * (*py++); tapCnt--; }
        py = pScratch2;
        *pOut++ = ssat16((int32_t)(acc0 >> 15));
    }
    return 0;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrcA, *pSrcB, *pDst, *pScratch1, *pScratch2;
    uint32_t srcALen, srcBLen, firstIndex, numPoints;
    arm_status status;
    (void)nlhs;

    if (nrhs != 4)
        mexErrMsgIdAndTxt("arm_conv_partial_opt_q15_mex:invalidInput",
            "Inputs: A(int16), B(int16), firstIndex(uint32), numPoints(uint32)");

    pSrcA = (int16_t *)mxGetData(prhs[0]);
    srcALen = (uint32_t)mxGetNumberOfElements(prhs[0]);
    pSrcB = (int16_t *)mxGetData(prhs[1]);
    srcBLen = (uint32_t)mxGetNumberOfElements(prhs[1]);

    if (mxIsDouble(prhs[2])) firstIndex = (uint32_t)((double *)mxGetData(prhs[2]))[0];
    else firstIndex = (uint32_t)((int32_t *)mxGetData(prhs[2]))[0];

    if (mxIsDouble(prhs[3])) numPoints = (uint32_t)((double *)mxGetData(prhs[3]))[0];
    else numPoints = (uint32_t)((int32_t *)mxGetData(prhs[3]))[0];

    {
        uint32_t totalLen = srcALen + srcBLen;
        pScratch1 = (int16_t *)mxCalloc(2 * totalLen, sizeof(int16_t));
        pScratch2 = (int16_t *)mxCalloc(totalLen, sizeof(int16_t));
    }

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)numPoints, mxINT16_CLASS, mxREAL);
    pDst = (int16_t *)mxGetData(plhs[0]);

    status = arm_conv_partial_opt_q15(pSrcA, srcALen, pSrcB, srcBLen, pDst, firstIndex, numPoints, pScratch1, pScratch2);

    if (status != 0)
        mexErrMsgIdAndTxt("arm_conv_partial_opt_q15_mex:rangeError",
            "firstIndex + numPoints exceeds output range.");

    mxFree(pScratch1);
    mxFree(pScratch2);
}