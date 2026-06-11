#include <stdint.h>

typedef int16_t q15_t;
typedef int32_t arm_status;

#define ARM_MATH_SUCCESS       0
#define ARM_MATH_SIZE_MISMATCH 1

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    q15_t *pData;
} arm_matrix_instance_q15;

arm_status arm_mat_sub_q15(
    const arm_matrix_instance_q15 *pSrcA,
    const arm_matrix_instance_q15 *pSrcB,
    arm_matrix_instance_q15 *pDst)
{
    q15_t *pInA = pSrcA->pData;
    q15_t *pInB = pSrcB->pData;
    q15_t *pOut = pDst->pData;
    uint32_t numSamples;
    uint32_t blkCnt;
    int32_t diff;

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
        diff = (int32_t)*pInA++ - (int32_t)*pInB++;

        if (diff > 32767)
            diff = 32767;
        else if (diff < -32768)
            diff = -32768;

        *pOut++ = (q15_t)diff;
        blkCnt--;
    }

    return ARM_MATH_SUCCESS;
}
