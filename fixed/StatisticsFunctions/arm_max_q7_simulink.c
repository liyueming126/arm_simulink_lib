#include <stdint.h>

typedef int8_t q7_t;

void arm_max_q7(
    q7_t *pSrc,
    uint32_t blockSize,
    q7_t *pResult,
    uint32_t *pIndex)
{
    q7_t maxVal, out;
    uint32_t blkCnt, outIndex;

    outIndex = 0u;
    out = *pSrc++;

    blkCnt = (blockSize - 1u);

    while (blkCnt > 0u)
    {
        maxVal = *pSrc++;

        if (out < maxVal)
        {
            out = maxVal;
            outIndex = blockSize - blkCnt;
        }

        blkCnt--;
    }

    *pResult = out;
    *pIndex = outIndex;
}
