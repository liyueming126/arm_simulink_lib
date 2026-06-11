#include <stdint.h>
#include <limits.h>

typedef int32_t q31_t;

void arm_add_q31(
    q31_t *pSrcA,
    q31_t *pSrcB,
    q31_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int64_t sum;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum = (int64_t)*pSrcA++ + (int64_t)*pSrcB++;

        if (sum > 2147483647LL)
            sum = 2147483647LL;
        else if (sum < -2147483648LL)
            sum = -2147483648LL;

        *pDst++ = (q31_t)sum;
        blkCnt--;
    }
}
