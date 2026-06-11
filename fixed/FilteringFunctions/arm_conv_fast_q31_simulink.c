#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

static void arm_conv_fast_q31(
    q31_t *pSrcA,
    uint32_t srcALen,
    q31_t *pSrcB,
    uint32_t srcBLen,
    q31_t *pDst)
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
                sum = (q31_t)((((q63_t)sum << 32) + ((q63_t)pSrcA[j] * pSrcB[i - j])) >> 32);
            }
        }

        pDst[i] = (q31_t)(sum << 1);
    }
}
