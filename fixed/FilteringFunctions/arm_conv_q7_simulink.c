#include <stdint.h>

typedef int8_t q7_t;

static void arm_conv_q7(
    q7_t *pSrcA,
    uint32_t srcALen,
    q7_t *pSrcB,
    uint32_t srcBLen,
    q7_t *pDst)
{
    int32_t sum;
    uint32_t i, j;

    for (i = 0; i < (srcALen + srcBLen - 1); i++)
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
        pDst[i] = (q7_t)sum;
    }
}
