#include <stdint.h>
#include <limits.h>

typedef int32_t q31_t;

void arm_scale_q31(
    q31_t *pSrc,
    q31_t scaleFract,
    int8_t shift,
    q31_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int64_t val;
    int8_t kShift;

    blkCnt = blockSize;
    kShift = 31 - shift;

    while (blkCnt > 0u)
    {
        val = ((int64_t)*pSrc++ * (int64_t)scaleFract) >> kShift;

        if (val > 2147483647LL)
            val = 2147483647LL;
        else if (val < -2147483648LL)
            val = -2147483648LL;

        *pDst++ = (q31_t)val;
        blkCnt--;
    }
}
