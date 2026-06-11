#include <stdint.h>

typedef int16_t q15_t;
typedef int32_t q31_t;

static void arm_conv_fast_q15(
    q15_t *pSrcA,
    uint32_t srcALen,
    q15_t *pSrcB,
    uint32_t srcBLen,
    q15_t *pDst)
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
                sum += (q31_t)pSrcA[j] * pSrcB[i - j];
            }
        }

        pDst[i] = (q15_t)(sum >> 15);
    }
}
