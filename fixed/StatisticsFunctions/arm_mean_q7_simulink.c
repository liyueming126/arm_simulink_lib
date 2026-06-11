#include <stdint.h>

typedef int8_t q7_t;
typedef int32_t q31_t;

void arm_mean_q7(
    q7_t *pSrc,
    uint32_t blockSize,
    q7_t *pResult)
{
    q31_t sum = 0;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum += *pSrc++;
        blkCnt--;
    }

    *pResult = (q7_t)(sum / (q31_t)blockSize);
}
