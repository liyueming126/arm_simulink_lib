#include <stdint.h>
#include <limits.h>

typedef int8_t q7_t;

void arm_add_q7(
    q7_t *pSrcA,
    q7_t *pSrcB,
    q7_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int16_t sum;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum = (int16_t)*pSrcA++ + (int16_t)*pSrcB++;

        if (sum > 127)
            sum = 127;
        else if (sum < -128)
            sum = -128;

        *pDst++ = (q7_t)sum;
        blkCnt--;
    }
}
