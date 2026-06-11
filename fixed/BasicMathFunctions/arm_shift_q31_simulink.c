#include <stdint.h>
#include <limits.h>

typedef int32_t q31_t;

void arm_shift_q31(
    q31_t *pSrc,
    int8_t shiftBits,
    q31_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;

    blkCnt = blockSize;

    if (shiftBits >= 0)
    {
        while (blkCnt > 0u)
        {
            int64_t val = (int64_t)*pSrc++ << shiftBits;

            if (val > 2147483647LL)
                val = 2147483647LL;
            else if (val < -2147483648LL)
                val = -2147483648LL;

            *pDst++ = (q31_t)val;
            blkCnt--;
        }
    }
    else
    {
        while (blkCnt > 0u)
        {
            *pDst++ = *pSrc++ >> (-shiftBits);
            blkCnt--;
        }
    }
}
