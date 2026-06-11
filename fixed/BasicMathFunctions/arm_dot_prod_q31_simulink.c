#include <stdint.h>
#include <limits.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_dot_prod_q31(
    q31_t *pSrcA,
    q31_t *pSrcB,
    uint32_t blockSize,
    q63_t *result)
{
    q63_t sum = 0;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum += ((q63_t)*pSrcA++ * (q63_t)*pSrcB++) >> 14u;
        blkCnt--;
    }

    *result = sum;
}
