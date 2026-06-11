#include <stdint.h>
#include <math.h>

void arm_std_f32(
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

    float meanOfSquares = sumOfSquares / ((float) blockSize - 1.0f);
    float mean = sum / (float) blockSize;
    float squareOfMean = (mean * mean) * (((float) blockSize) / ((float) blockSize - 1.0f));

    *pResult = sqrtf(meanOfSquares - squareOfMean);
}