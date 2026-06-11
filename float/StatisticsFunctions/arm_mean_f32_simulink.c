#include <stdint.h>

void arm_mean_f32(
  float * pSrc,
  uint32_t blockSize,
  float * pResult)
{
    float sum = 0.0f;
    uint32_t blkCnt;

    blkCnt = blockSize;
    while(blkCnt > 0u)
    {
        sum += *pSrc++;
        blkCnt--;
    }

    *pResult = sum / (float) blockSize;
}