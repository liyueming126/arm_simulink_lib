#include <stdint.h>

typedef int16_t q15_t;

void arm_min_q15(
    q15_t *pSrc,
    uint32_t blockSize,
    q15_t *pResult,
    uint32_t *pIndex)
{
    q15_t minVal, out;
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
