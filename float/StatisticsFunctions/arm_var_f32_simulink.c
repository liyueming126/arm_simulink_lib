#include <stdint.h>

void arm_var_f32(
  float * pSrc,
  uint32_t blockSize,
  float * pResult)
{
    float sum = 0.0f;
    float sumOfSquares = 0.0f;
    float in;
    uint32_t blkCnt;

    if(blockSize == 1)
    {
        *pResult = 0;
        return;
    }

    blkCnt = blockSize;
    while(blkCnt > 0u)
    {
        in = *pSrc++;
        sumOfSquares += in * in;
        sum += in;
        blkCnt--;
    }

    float squareOfSum = (sum * sum) / (float) blockSize;
    *pResult = (sumOfSquares - squareOfSum) / (float) (blockSize - 1u);
}