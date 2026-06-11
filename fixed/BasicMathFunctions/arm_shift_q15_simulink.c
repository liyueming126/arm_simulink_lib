#include <stdint.h>
#include <limits.h>

typedef int16_t q15_t;

void arm_shift_q15(
    q15_t *pSrc,
    int8_t shiftBits,
    q15_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;

    blkCnt = blockSize;

    if (shiftBits >= 0)
    {
        while (blkCnt > 0u)
        {
            int32_t val = ((int32_t)*pSrc++) << shiftBits;

            if (val > 32767)
                val = 32767;
            else if (val < -32768)
                val = -32768;

            *pDst++ = (q15_t)val;
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
