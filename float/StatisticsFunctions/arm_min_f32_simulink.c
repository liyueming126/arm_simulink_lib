#include <stdint.h>

void arm_min_f32(
  float * pSrc,
  uint32_t blockSize,
  float * pResult,
  uint32_t * pIndex)
{
    float minVal1, out;
    uint32_t blkCnt, outIndex;

    outIndex = 0u;
    out = *pSrc++;
    blkCnt = (blockSize - 1u);

    while(blkCnt > 0u)
    {
        minVal1 = *pSrc++;
        if(out > minVal1)
        {
            out = minVal1;
            outIndex = blockSize - blkCnt;
        }
        blkCnt--;
    }

    *pResult = out;
    *pIndex = outIndex;
}