#include <stdint.h>
#include <limits.h>

typedef int8_t q7_t;
typedef int32_t q31_t;

void arm_dot_prod_q7(
    q7_t *pSrcA,
    q7_t *pSrcB,
    uint32_t blockSize,
    q31_t *result)
{
    q31_t sum = 0;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum += (q31_t)((int16_t)*pSrcA++ * (int16_t)*pSrcB++);
        blkCnt--;
    }

    *result = sum;
}
