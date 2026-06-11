#include <stdint.h>

typedef int16_t q15_t;

void arm_max_q15(
    q15_t *pSrc,
    uint32_t blockSize,
    q15_t *pResult,
    uint32_t *pIndex)
{
    q15_t maxVal, out;
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
