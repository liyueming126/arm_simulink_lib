#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

static void arm_correlate_q31(
    q31_t *pSrcA,
    uint32_t srcALen,
    q31_t *pSrcB,
    uint32_t srcBLen,
    q31_t *pDst)
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
                sum += (q63_t)pSrcA[n] * pSrcB[idxB];
            }
        }

        pDst[k] = (q31_t)(sum >> 31u);
    }
}
