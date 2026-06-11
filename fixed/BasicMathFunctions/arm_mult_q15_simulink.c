#include <stdint.h>
#include <limits.h>

typedef int16_t q15_t;

void arm_mult_q15(
    q15_t *pSrcA,
    q15_t *pSrcB,
    q15_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int32_t prod;

    blkCnt = blockSize;

    while (blkCnt > 0u)
	{
		prod = (int32_t)*pSrcA++ * (int32_t)*pSrcB++;
		prod = prod >> 15;

		if (prod > 32767)
			prod = 32767;
		else if (prod < -32768)
			prod = -32768;

		*pDst++ = (q15_t)prod;
		blkCnt--;
	}
}
