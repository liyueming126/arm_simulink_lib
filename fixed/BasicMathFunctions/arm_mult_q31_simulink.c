#include <stdint.h>
#include <limits.h>

typedef int32_t q31_t;

void arm_mult_q31(
    q31_t *pSrcA,
    q31_t *pSrcB,
    q31_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int64_t prod;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        prod = (int64_t)*pSrcA++ * (int64_t)*pSrcB++;
        prod = prod >> 31;

        if (prod > 2147483647LL)
            prod = 2147483647LL;
        else if (prod < -2147483648LL)
            prod = -2147483648LL;

        *pDst++ = (q31_t)prod;
        blkCnt--;
    }
}
