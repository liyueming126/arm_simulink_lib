#include <stdint.h>
#include <limits.h>

typedef int8_t q7_t;

void arm_mult_q7(
    q7_t *pSrcA,
    q7_t *pSrcB,
    q7_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int16_t prod;

    blkCnt = blockSize;

    while (blkCnt > 0u)
	{
		prod = ((int16_t)*pSrcA++ * (int16_t)*pSrcB++) >> 7;

		if (prod > 127)
			prod = 127;
		else if (prod < -128)
			prod = -128;

		*pDst++ = (q7_t)prod;
		blkCnt--;
	}
}
