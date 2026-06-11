#include <stdint.h>
#include <limits.h>

typedef int16_t q15_t;
typedef int64_t q63_t;

void arm_dot_prod_q15(
    q15_t *pSrcA,
    q15_t *pSrcB,
    uint32_t blockSize,
    q63_t *result)
{
    q63_t sum = 0;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum += (q63_t)((int32_t)*pSrcA++ * (int32_t)*pSrcB++);
        blkCnt--;
    }

    *result = sum;
}
