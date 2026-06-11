#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

static int16_t ssat16(int32_t val)
{
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (int16_t)val;
}

static void arm_correlate_fast_opt_q15(
    q15_t *pSrcA, uint32_t srcALen,
    q15_t *pSrcB, uint32_t srcBLen,
    q15_t *pDst,
    q15_t *pScratch)
{
    q15_t *pIn1, *pIn2, *pScr, *py, *pOut = pDst;
    uint32_t j, blkCnt, tapCnt;
    q31_t acc0;
    int32_t inc = 1;

    if (srcALen >= srcBLen) {
        pIn1 = pSrcA; pIn2 = pSrcB;
        j = (2u * srcALen) - 1u;
        j = j - (srcALen + srcBLen - 1u);
        pOut += j;
    } else {
        pIn1 = pSrcB; pIn2 = pSrcA;
        j = srcBLen; srcBLen = srcALen; srcALen = j;
        pOut = pDst + ((srcALen + srcBLen) - 2u);
        inc = -1;
    }

    pScr = pScratch;
    for (j = 0; j < srcBLen - 1u; j++) pScr[j] = 0;
    pScr += (srcBLen - 1u);
    for (j = 0; j < srcALen; j++) pScr[j] = pIn1[j];
    pScr += srcALen;
    for (j = 0; j < srcBLen - 1u; j++) pScr[j] = 0;

    py = pIn2;
    blkCnt = srcALen + srcBLen - 1u;

    while (blkCnt > 0u) {
        pScr = pScratch;
        acc0 = 0;
        tapCnt = srcBLen;
        while (tapCnt > 0u) { acc0 += (q31_t)(*pScr++) * (*py++); tapCnt--; }
        py = pIn2;
        *pOut = ssat16(acc0 >> 15);
        pOut += inc;
        pScratch++;
        blkCnt--;
    }
}
