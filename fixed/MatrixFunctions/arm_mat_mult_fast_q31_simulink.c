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

arm_status arm_mat_mult_fast_q31(
    const arm_matrix_instance_q31 *pSrcA,
    const arm_matrix_instance_q31 *pSrcB,
    arm_matrix_instance_q31 *pDst)
{
    q31_t *pA = pSrcA->pData;
    q31_t *pB = pSrcB->pData;
    q31_t *pC = pDst->pData;
    uint16_t numRowsA = pSrcA->numRows;
    uint16_t numColsA = pSrcA->numCols;
    uint16_t numRowsB = pSrcB->numRows;
    uint16_t numColsB = pSrcB->numCols;
    uint16_t r, c, k;

    if ((pSrcA->numCols != pSrcB->numRows) ||
        (pSrcA->numRows != pDst->numRows) ||
        (pSrcB->numCols != pDst->numCols))
        return ARM_MATH_SIZE_MISMATCH;

    /* Column-major matrix multiply using 64-bit accumulator */
    for (r = 0; r < numRowsA; r++)
    {
        for (c = 0; c < numColsB; c++)
        {
            q63_t sum = 0;
            for (k = 0; k < numColsA; k++)
            {
                sum += (q63_t)pA[r + k * numRowsA] * (q63_t)pB[k + c * numRowsB];
            }
            /* result in 1.31 format: shift right 31 and saturate */
            {
                q31_t res = (q31_t)(sum >> 31);
                pC[r + c * numRowsA] = res;
            }
        }
    }

    return ARM_MATH_SUCCESS;
}
