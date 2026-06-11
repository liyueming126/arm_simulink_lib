#include <stdint.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

static void arm_correlate_q15(
    q15_t *pSrcA,
    uint32_t srcALen,
    q15_t *pSrcB,
    uint32_t srcBLen,
    q15_t *pDst)
{
    q63_t sum;
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
                sum += (q31_t)pSrcA[n] * pSrcB[idxB];
            }
        }

        if (sum > 0)
        {
            sum = (sum >> 15);
            pDst[k] = (q15_t)(sum > 32767 ? 32767 : sum);
        }
        else
        {
            sum = (sum >> 15);
            pDst[k] = (q15_t)(sum < -32768 ? -32768 : sum);
        }
    }
}
