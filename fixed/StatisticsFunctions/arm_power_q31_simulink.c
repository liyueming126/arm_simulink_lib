#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_power_q31(
    q31_t *pSrc,
    uint32_t blockSize,
    q63_t *pResult)
{
    q63_t sum = 0;
    q31_t in;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        sum += ((q63_t)in * in) >> 14u;
        blkCnt--;
    }

    *pResult = sum;
}
