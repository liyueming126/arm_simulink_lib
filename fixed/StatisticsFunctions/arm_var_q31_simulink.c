#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_var_q31(
    q31_t *pSrc,
    uint32_t blockSize,
    q31_t *pResult)
{
    q63_t sum = 0;
    q63_t meanOfSquares, squareOfMean;
    q31_t in;
    uint32_t blkCnt;
    q63_t sumOfSquares = 0;

    if (blockSize == 1)
    {
        *pResult = 0;
        return;
    }

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++ >> 8;
        sumOfSquares += ((q63_t)in * in);
        sum += in;
        blkCnt--;
    }

    meanOfSquares = sumOfSquares / (q63_t)(blockSize - 1u);
    squareOfMean = sum * sum / (q63_t)(blockSize * (blockSize - 1u));

    *pResult = (q31_t)((meanOfSquares - squareOfMean) >> 15);
}
