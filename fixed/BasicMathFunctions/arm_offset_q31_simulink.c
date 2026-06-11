#include <stdint.h>
#include <limits.h>

typedef int32_t q31_t;

void arm_offset_q31(
    q31_t *pSrc,
    q31_t offset,
    q31_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int64_t val;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        val = (int64_t)*pSrc++ + (int64_t)offset;

        if (val > 2147483647LL)
            val = 2147483647LL;
        else if (val < -2147483648LL)
            val = -2147483648LL;

        *pDst++ = (q31_t)val;
        blkCnt--;
    }
}
