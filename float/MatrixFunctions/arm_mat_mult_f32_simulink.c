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

arm_status arm_mat_mult_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
  arm_matrix_instance_f32 * pDst)
{
    float *pIn1 = pSrcA->pData;
    float *pIn2 = pSrcB->pData;
    float *pInA = pSrcA->pData;
    float *pOut = pDst->pData;
    float *px;
    float sum;
    uint16_t numRowsA = pSrcA->numRows;
    uint16_t numColsB = pSrcB->numCols;
    uint16_t numColsA = pSrcA->numCols;
    float *pInB = pSrcB->pData;
    uint16_t col, i = 0u, row = numRowsA, colCnt;
    arm_status status;

    if((pSrcA->numCols != pSrcB->numRows) ||
       (pSrcA->numRows != pDst->numRows) || (pSrcB->numCols != pDst->numCols))
    {
        status = ARM_MATH_SIZE_MISMATCH;
    }
    else
    {
        do
        {
            px = pOut + i;
            col = numColsB;
            pIn2 = pSrcB->pData;
            do
            {
                sum = 0.0f;
                pIn1 = pInA;
                colCnt = numColsA;
                while(colCnt > 0u)
                {
                    sum += *pIn1++ * (*pIn2);
                    pIn2 += numColsB;
                    colCnt--;
                }
                *px++ = sum;
                col--;
                pIn2 = pInB + (numColsB - col);
            } while(col > 0u);

            i = i + numColsB;
            pInA = pInA + numColsA;
            row--;
        } while(row > 0u);
        status = ARM_MATH_SUCCESS;
    }
    return status;
}