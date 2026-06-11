#include <stdint.h>
#include <limits.h>

typedef int16_t q15_t;

void arm_offset_q15(
    q15_t *pSrc,
    q15_t offset,
    q15_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int32_t val;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        val = (int32_t)*pSrc++ + (int32_t)offset;

        if (val > 32767)
            val = 32767;
        else if (val < -32768)
            val = -32768;

        *pDst++ = (q15_t)val;
        blkCnt--;
    }
}
