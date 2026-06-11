#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_mean_q31(
    q31_t *pSrc,
    uint32_t blockSize,
    q31_t *pResult)
{
    q63_t sum = 0;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum += *pSrc++;
        blkCnt--;
    }

    *pResult = (q31_t)(sum / (int32_t)blockSize);
}
