#include <stdint.h>

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    float *pData;
} arm_matrix_instance_f32;

typedef enum {
    ARM_MATH_SUCCESS = 0,
    ARM_MATH_SIZE_MISMATCH = -1,
    ARM_MATH_SINGULAR = -2
} arm_status;

arm_status arm_mat_trans_f32(
  const arm_matrix_instance_f32 * pSrc,
  arm_matrix_instance_f32 * pDst)
{
    float *pIn = pSrc->pData;
    float *pOut = pDst->pData;
    float *px;
    uint16_t nRows = pSrc->numRows;
    uint16_t nColumns = pSrc->numCols;
    uint16_t col, i = 0u, row = nRows;
    arm_status status;

    if((pSrc->numRows != pDst->numCols) || (pSrc->numCols != pDst->numRows))
    {
        status = ARM_MATH_SIZE_MISMATCH;
    }
    else
    {
        do
        {
            px = pOut + i;
            col = nColumns;
            while(col > 0u)
            {
                *px = *pIn++;
                px += nRows;
                col--;
            }
            i++;
            row--;
        } while(row > 0u);
        status = ARM_MATH_SUCCESS;
    }
    return status;
}