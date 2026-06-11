#include <stdint.h>
#include <limits.h>

typedef int8_t q7_t;

void arm_negate_q7(
    q7_t *pSrc,
    q7_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    q7_t in;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        *pDst++ = (in == INT8_MIN) ? INT8_MAX : (q7_t)(-in);
        blkCnt--;
    }
}
