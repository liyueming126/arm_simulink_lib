#include <stdint.h>
#include <string.h>

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

static void arm_correlate_opt_q7(
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
    int32_t inc = 1;
    q7_t *pOut = pDst;
    uint32_t outBlockSize;

    if (srcALen >= srcBLen)
    {
        pIn1 = pSrcA;
        pIn2 = pSrcB;
        outBlockSize = (2u * srcALen) - 1u;
        j = outBlockSize - (srcALen + (srcBLen - 1u));
        pOut += j;
    }
    else
    {
        pIn1 = pSrcB;
        pIn2 = pSrcA;
        j = srcBLen;
        srcBLen = srcALen;
        srcALen = j;
        pOut = pDst + ((srcALen + srcBLen) - 2u);
        inc = -1;
    }

    k = srcBLen;
    while (k > 0u)
    {
        *pScr2++ = (q15_t)*pIn2++;
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
        *pOut = (q7_t)acc0;
        pOut += inc;

        pScr2 = py;
        pScratch1 += 1u;
        blkCnt--;
    }
}
