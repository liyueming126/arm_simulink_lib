#include <stdint.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int32_t arm_status;

#define ARM_MATH_SUCCESS       0
#define ARM_MATH_SIZE_MISMATCH 1

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    q15_t *pData;
} arm_matrix_instance_q15;

arm_status arm_mat_mult_fast_q15(
    const arm_matrix_instance_q15 *pSrcA,
    const arm_matrix_instance_q15 *pSrcB,
    arm_matrix_instance_q15 *pDst,
    q15_t *pState)
{
    q15_t *pA = pSrcA->pData;
    q15_t *pB = pSrcB->pData;
    q15_t *pC = pDst->pData;
    uint16_t numRowsA = pSrcA->numRows;
    uint16_t numColsA = pSrcA->numCols;
    uint16_t numRowsB = pSrcB->numRows;
    uint16_t numColsB = pSrcB->numCols;
    uint16_t r, c, k;
    q31_t sum;
    (void)pState;

    if ((pSrcA->numCols != pSrcB->numRows) ||
        (pSrcA->numRows != pDst->numRows) ||
        (pSrcB->numCols != pDst->numCols))
        return ARM_MATH_SIZE_MISMATCH;

    /* Column-major matrix multiply with 32-bit accumulator */
    for (r = 0; r < numRowsA; r++)
    {
        for (c = 0; c < numColsB; c++)
        {
            sum = 0;
            for (k = 0; k < numColsA; k++)
            {
                sum += (q31_t)pA[r + k * numRowsA] * (q31_t)pB[k + c * numRowsB];
            }
            sum = sum >> 15;
            if (sum > 32767) sum = 32767;
            else if (sum < -32768) sum = -32768;
            pC[r + c * numRowsA] = (q15_t)sum;
        }
    }

    return ARM_MATH_SUCCESS;
}
