#include <stdint.h>

typedef float float32_t;

void arm_dot_prod_f32(
    float32_t *pSrcA,
    float32_t *pSrcB,
    uint32_t blockSize,
    float32_t *result)
{
    float32_t sum = 0.0f;
    uint32_t blkCnt;
    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum += (*pSrcA++) * (*pSrcB++);
        blkCnt--;
    }
    *result = sum;
}