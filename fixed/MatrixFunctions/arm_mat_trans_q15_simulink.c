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

arm_status arm_mat_trans_q15(
    const arm_matrix_instance_q15 *pSrc,
    arm_matrix_instance_q15 *pDst)
{
    q15_t *pSrcA = pSrc->pData;
    q15_t *pOut;
    uint16_t nRows = pSrc->numRows;
    uint16_t nColumns = pSrc->numCols;
    uint16_t col, row, i;

    if ((pSrc->numRows != pDst->numCols) || (pSrc->numCols != pDst->numRows))
        return ARM_MATH_SIZE_MISMATCH;

    row = nRows;
    i = 0u;

    do
    {
        pOut = pDst->pData + i;
        col = nColumns;

        while (col > 0u)
        {
            *pOut = *pSrcA++;
            pOut += nRows;
            col--;
        }

        i++;
        row--;
    } while (row > 0u);

    return ARM_MATH_SUCCESS;
}
