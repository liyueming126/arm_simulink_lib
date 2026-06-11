#include <stdint.h>
#include <limits.h>

typedef int8_t q7_t;

void arm_shift_q7(
    q7_t *pSrc,
    int8_t shiftBits,
    q7_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;

    blkCnt = blockSize;

    if (shiftBits >= 0)
    {
        while (blkCnt > 0u)
        {
            int16_t val = ((int16_t)*pSrc++) << shiftBits;

            if (val > 127)
                val = 127;
            else if (val < -128)
                val = -128;

            *pDst++ = (q7_t)val;
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
