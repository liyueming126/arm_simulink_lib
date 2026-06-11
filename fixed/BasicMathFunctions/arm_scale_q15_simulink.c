#include <stdint.h>
#include <limits.h>

typedef int16_t q15_t;

void arm_scale_q15(
    q15_t *pSrc,
    q15_t scaleFract,
    int8_t shift,
    q15_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int32_t val;
    int8_t kShift;

    blkCnt = blockSize;
    kShift = 15 - shift;

    while (blkCnt > 0u)
	{
		val = ((int32_t)*pSrc++ * (int32_t)scaleFract) >> kShift;

		if (val > 32767)
			val = 32767;
		else if (val < -32768)
			val = -32768;

		*pDst++ = (q15_t)val;
		blkCnt--;
	}
}
