#include <stdint.h>
#include <limits.h>

typedef int32_t q31_t;

void arm_abs_q31(
    q31_t *pSrc,
    q31_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    q31_t in;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : (q31_t)(-in));
        blkCnt--;
    }
}
