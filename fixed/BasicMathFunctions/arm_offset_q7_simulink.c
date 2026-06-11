#include <stdint.h>
#include <limits.h>

typedef int8_t q7_t;

void arm_offset_q7(
    q7_t *pSrc,
    q7_t offset,
    q7_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int16_t val;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        val = (int16_t)*pSrc++ + (int16_t)offset;

        if (val > 127)
            val = 127;
        else if (val < -128)
            val = -128;

        *pDst++ = (q7_t)val;
        blkCnt--;
    }
}
