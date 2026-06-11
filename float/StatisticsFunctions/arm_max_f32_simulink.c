#include <stdint.h>

void arm_max_f32(
  float * pSrc,
  uint32_t blockSize,
  float * pResult,
  uint32_t * pIndex)
{
    float maxVal1, out;
    uint32_t blkCnt, outIndex;

    outIndex = 0u;
    out = *pSrc++;
    blkCnt = (blockSize - 1u);

    while(blkCnt > 0u)
    {
        maxVal1 = *pSrc++;
        if(out < maxVal1)
        {
            out = maxVal1;
            outIndex = blockSize - blkCnt;
        }
        blkCnt--;
    }

    *pResult = out;
    *pIndex = outIndex;
}