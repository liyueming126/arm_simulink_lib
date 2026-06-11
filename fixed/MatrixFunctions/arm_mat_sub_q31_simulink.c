#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;
typedef int32_t arm_status;

#define ARM_MATH_SUCCESS       0
#define ARM_MATH_SIZE_MISMATCH 1

typedef struct {
	uint16_t numRows;
	uint16_t numCols;
	q31_t *pData;
} arm_matrix_instance_q31;

arm_status arm_mat_sub_q31(
	const arm_matrix_instance_q31 *pSrcA,
	const arm_matrix_instance_q31 *pSrcB,
	arm_matrix_instance_q31 *pDst)
{
	q31_t *pIn1 = pSrcA->pData;
	q31_t *pIn2 = pSrcB->pData;
	q31_t *pOut = pDst->pData;
	uint32_t numSamples;
	uint32_t blkCnt;
	q63_t diff;

	if ((pSrcA->numRows != pSrcB->numRows) ||
		(pSrcA->numCols != pSrcB->numCols) ||
		(pSrcA->numRows != pDst->numRows) ||
		(pSrcA->numCols != pDst->numCols))
	{
		return ARM_MATH_SIZE_MISMATCH;
	}

	numSamples = (uint32_t)pSrcA->numRows * pSrcA->numCols;
	blkCnt = numSamples;

	while (blkCnt > 0u)
	{
		diff = (q63_t)*pIn1++ - (q63_t)*pIn2++;

		if (diff > (q63_t)INT32_MAX)
			diff = INT32_MAX;
		else if (diff < (q63_t)INT32_MIN)
			diff = INT32_MIN;

		*pOut++ = (q31_t)diff;
		blkCnt--;
	}

	return ARM_MATH_SUCCESS;
}
