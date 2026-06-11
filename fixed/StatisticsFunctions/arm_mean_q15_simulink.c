#include <stdint.h>

typedef int16_t q15_t;
typedef int32_t q31_t;

void arm_mean_q15(
    q15_t *pSrc,
    uint32_t blockSize,
    q15_t *pResult)
{
    q31_t sum = 0;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum += *pSrc++;
        blkCnt--;
    }

    *pResult = (q15_t)(sum / (q31_t)blockSize);
}
