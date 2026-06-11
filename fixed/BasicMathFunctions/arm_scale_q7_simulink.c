#include <stdint.h>
#include <limits.h>

typedef int8_t q7_t;

void arm_scale_q7(
    q7_t *pSrc,
    q7_t scaleFract,
    int8_t shift,
    q7_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int16_t val;
    int8_t kShift;

    blkCnt = blockSize;
    kShift = 7 - shift;

    while (blkCnt > 0u)
	{
		val = ((int16_t)*pSrc++ * (int16_t)scaleFract) >> kShift;

		if (val > 127)
			val = 127;
		else if (val < -128)
			val = -128;

		*pDst++ = (q7_t)val;
		blkCnt--;
	}
}
