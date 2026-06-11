#include <stdint.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_var_q15(
    q15_t *pSrc,
    uint32_t blockSize,
    q15_t *pResult)
{
    q31_t sum = 0;
    q31_t meanOfSquares, squareOfMean;
    uint32_t blkCnt;
    q63_t sumOfSquares = 0;
    q15_t in;

    if (blockSize == 1)
    {
        *pResult = 0;
        return;
    }

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        sumOfSquares += ((q63_t)in * in);
        sum += in;
        blkCnt--;
    }

    meanOfSquares = (q31_t)(sumOfSquares / (q63_t)(blockSize - 1u));
    squareOfMean = (q31_t)(((q63_t)sum * sum) / (q63_t)(blockSize * (blockSize - 1u)));

    *pResult = (q15_t)((meanOfSquares - squareOfMean) >> 15);
}
