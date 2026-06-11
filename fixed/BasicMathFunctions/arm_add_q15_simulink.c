#include <stdint.h>
#include <limits.h>

typedef int16_t q15_t;

void arm_add_q15(
    q15_t *pSrcA,
    q15_t *pSrcB,
    q15_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int32_t sum;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum = (int32_t)*pSrcA++ + (int32_t)*pSrcB++;

        if (sum > 32767)
            sum = 32767;
        else if (sum < -32768)
            sum = -32768;

        *pDst++ = (q15_t)sum;
        blkCnt--;
    }
}
