#include <stdint.h>
#include <limits.h>

typedef int32_t q31_t;

void arm_sub_q31(
    q31_t *pSrcA,
    q31_t *pSrcB,
    q31_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int64_t diff;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        diff = (int64_t)*pSrcA++ - (int64_t)*pSrcB++;

        if (diff > 2147483647LL)
            diff = 2147483647LL;
        else if (diff < -2147483648LL)
            diff = -2147483648LL;

        *pDst++ = (q31_t)diff;
        blkCnt--;
    }
}
