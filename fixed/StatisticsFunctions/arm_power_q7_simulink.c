#include <stdint.h>

typedef int8_t q7_t;
typedef int32_t q31_t;

void arm_power_q7(
    q7_t *pSrc,
    uint32_t blockSize,
    q31_t *pResult)
{
    q31_t sum = 0;
    q7_t in;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        sum += ((int16_t)in * in);
        blkCnt--;
    }

    *pResult = sum;
}
