#include <stdint.h>

typedef float float32_t;

void arm_mult_f32(
    float32_t *pSrcA,
    float32_t *pSrcB,
    float32_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    blkCnt = blockSize;
    while (blkCnt > 0u)
    {
        *pDst++ = (*pSrcA++) * (*pSrcB++);
        blkCnt--;
    }
}