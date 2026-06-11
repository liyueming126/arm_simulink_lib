#include <stdint.h>

typedef int8_t q7_t;

static void arm_correlate_q7(
    q7_t *pSrcA,
    uint32_t srcALen,
    q7_t *pSrcB,
    uint32_t srcBLen,
    q7_t *pDst)
{
    int32_t sum;
    int32_t k, n;
    int32_t idxB;
    int32_t sALen = (int32_t)srcALen;
    int32_t sBLen = (int32_t)srcBLen;
    int32_t total = sALen + sBLen - 1;

    for (k = 0; k < total; k++)
    {
        sum = 0;

        for (n = 0; n < sALen; n++)
        {
            idxB = n - k + sBLen - 1;
            if (idxB >= 0 && idxB < sBLen)
            {
                sum += (int16_t)pSrcA[n] * (int16_t)pSrcB[idxB];
            }
        }

        sum = sum >> 7;
        if (sum > 127) sum = 127;
        if (sum < -128) sum = -128;
        pDst[k] = (q7_t)sum;
    }
}
