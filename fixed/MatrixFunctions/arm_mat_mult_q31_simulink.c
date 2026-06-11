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

static q31_t clip_q63_to_q31(q63_t x)
{
    if (x > 2147483647LL)
        return 2147483647;
    if (x < (-2147483647LL - 1))
        return (-2147483647 - 1);
    return (q31_t)x;
}

arm_status arm_mat_mult_q31(
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

    /* Column-major matrix multiply: C[r][c] = sum_k A[r][k] * B[k][c]
       A[r][k] at index r + k * numRowsA
       B[k][c] at index k + c * numRowsB  */
    for (r = 0; r < numRowsA; r++)
    {
        for (c = 0; c < numColsB; c++)
        {
            q63_t sum = 0;
            for (k = 0; k < numColsA; k++)
            {
                sum += (q63_t)pA[r + k * numRowsA] * (q63_t)pB[k + c * numRowsB];
            }
            pC[r + c * numRowsA] = clip_q63_to_q31(sum >> 31);
        }
    }

    return ARM_MATH_SUCCESS;
}
