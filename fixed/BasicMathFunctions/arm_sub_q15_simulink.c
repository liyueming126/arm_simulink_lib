#include <stdint.h>
#include <limits.h>

typedef int16_t q15_t;

void arm_sub_q15(
    q15_t *pSrcA,
    q15_t *pSrcB,
    q15_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int32_t diff;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        diff = (int32_t)*pSrcA++ - (int32_t)*pSrcB++;

        if (diff > 32767)
            diff = 32767;
        else if (diff < -32768)
            diff = -32768;

        *pDst++ = (q15_t)diff;
        blkCnt--;
    }
}
