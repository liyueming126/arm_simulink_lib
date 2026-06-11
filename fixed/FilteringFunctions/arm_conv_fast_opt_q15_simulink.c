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

static void arm_conv_fast_opt_q15(
    q15_t *pSrcA, uint32_t srcALen,
    q15_t *pSrcB, uint32_t srcBLen,
    q15_t *pDst,
    q15_t *pScratch1, q15_t *pScratch2)
{
    q15_t *pIn1, *pIn2, *px, *py, *pScr1;
    uint32_t j, k, blkCnt, tapCnt;
    q31_t acc0;
    q15_t *pOut = pDst;

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
    k = srcALen + srcBLen - 1u;
    for (blkCnt = 0; blkCnt < k; blkCnt++)
    {
        pScr1 = pScratch1 + blkCnt;
        acc0 = 0;
        tapCnt = srcBLen;
        while (tapCnt > 0u) { acc0 += (q31_t)(*pScr1++) * (*py++); tapCnt--; }
        py = pScratch2;
        *pOut++ = ssat16(acc0 >> 15);
    }
}
