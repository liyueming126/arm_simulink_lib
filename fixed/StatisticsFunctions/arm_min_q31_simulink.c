#include <stdint.h>

typedef int32_t q31_t;

void arm_min_q31(
    q31_t *pSrc,
    uint32_t blockSize,
    q31_t *pResult,
    uint32_t *pIndex)
{
    q31_t minVal, out;
    uint32_t blkCnt, outIndex;

    outIndex = 0u;
    out = *pSrc++;

    blkCnt = (blockSize - 1u);

    while (blkCnt > 0u)
    {
        minVal = *pSrc++;

        if (out > minVal)
        {
            out = minVal;
            outIndex = blockSize - blkCnt;
        }

        blkCnt--;
    }

    *pResult = out;
    *pIndex = outIndex;
}
