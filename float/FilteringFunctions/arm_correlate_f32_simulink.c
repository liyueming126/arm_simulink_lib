/* Simplified implementation of ARM correlation for Simulink.
 * Based on CMSIS arm_correlate_f32.c (Cortex-M0 version).
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;

void arm_correlate_f32(
    float32_t * pSrcA,
    uint32_t srcALen,
    float32_t * pSrcB,
    uint32_t srcBLen,
    float32_t * pDst)
{
    float32_t *pIn1 = pSrcA;
    float32_t *pIn2 = pSrcB + (srcBLen - 1u);
    float32_t sum;
    uint32_t i = 0u, j;
    uint32_t inv = 0u;
    uint32_t tot = ((srcALen + srcBLen) - 2u);

    if (srcALen > srcBLen) {
        j = srcALen - srcBLen;
        pDst += j;
    } else if (srcALen < srcBLen) {
        pIn1 = pSrcB;
        pIn2 = pSrcA + (srcALen - 1u);
        pDst = pDst + tot;
        j = srcALen;
        srcALen = srcBLen;
        srcBLen = j;
        inv = 1;
    }

    for (i = 0u; i <= tot; i++)
    {
        sum = 0.0f;
        for (j = 0u; j <= i; j++)
        {
            if (((i - j) < srcBLen) && (j < srcALen))
            {
                sum += pIn1[j] * pIn2[-((int32_t)i - j)];
            }
        }
        if (inv == 1)
            *pDst-- = sum;
        else
            *pDst++ = sum;
    }
}