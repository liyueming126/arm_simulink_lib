#include <stdint.h>
#include <limits.h>

typedef int8_t q7_t;

void arm_sub_q7(
    q7_t *pSrcA,
    q7_t *pSrcB,
    q7_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int16_t diff;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        diff = (int16_t)*pSrcA++ - (int16_t)*pSrcB++;

        if (diff > 127)
            diff = 127;
        else if (diff < -128)
            diff = -128;

        *pDst++ = (q7_t)diff;
        blkCnt--;
    }
}
