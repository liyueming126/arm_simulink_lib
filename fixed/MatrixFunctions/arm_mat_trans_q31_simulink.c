#include <stdint.h>

typedef int32_t q31_t;
typedef int32_t arm_status;

#define ARM_MATH_SUCCESS       0
#define ARM_MATH_SIZE_MISMATCH 1

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    q31_t *pData;
} arm_matrix_instance_q31;

arm_status arm_mat_trans_q31(
    const arm_matrix_instance_q31 *pSrc,
    arm_matrix_instance_q31 *pDst)
{
    q31_t *pIn = pSrc->pData;
    q31_t *px;
    uint16_t nRows = pSrc->numRows;
    uint16_t nColumns = pSrc->numCols;
    uint16_t col, row, i;

    if ((pSrc->numRows != pDst->numCols) || (pSrc->numCols != pDst->numRows))
        return ARM_MATH_SIZE_MISMATCH;

    row = nRows;
    i = 0u;

    do
    {
        px = pDst->pData + i;
        col = nColumns;

        while (col > 0u)
        {
            *px = *pIn++;
            px += nRows;
            col--;
        }

        i++;
        row--;
    } while (row > 0u);

    return ARM_MATH_SUCCESS;
}
