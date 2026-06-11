#include <stdint.h>
#include <limits.h>

typedef int16_t q15_t;

void arm_negate_q15(
    q15_t *pSrc,
    q15_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    q15_t in;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        *pDst++ = (in == INT16_MIN) ? INT16_MAX : (q15_t)(-in);
        blkCnt--;
    }
}
