#include <stdint.h>

typedef int16_t q15_t;
typedef int64_t q63_t;

void arm_power_q15(
    q15_t *pSrc,
    uint32_t blockSize,
    q63_t *pResult)
{
    q63_t sum = 0;
    q15_t in;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        sum += ((int32_t)in * in);
        blkCnt--;
    }

    *pResult = sum;
}
