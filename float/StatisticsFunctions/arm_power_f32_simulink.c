#include <stdint.h>

void arm_power_f32(
  float * pSrc,
  uint32_t blockSize,
  float * pResult)
{
    float sum = 0.0f;
    float in;
    uint32_t blkCnt;

    blkCnt = blockSize;
    while(blkCnt > 0u)
    {
        in = *pSrc++;
        sum += in * in;
        blkCnt--;
    }

    *pResult = sum;
}