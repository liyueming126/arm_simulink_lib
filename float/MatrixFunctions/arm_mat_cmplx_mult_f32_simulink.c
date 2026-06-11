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

arm_status arm_mat_cmplx_mult_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
  arm_matrix_instance_f32 * pDst)
{
    float *pIn1 = pSrcA->pData;
    float *pIn2 = pSrcB->pData;
    float *pInA = pSrcA->pData;
    float *pOut = pDst->pData;
    float *px;
    uint16_t numRowsA = pSrcA->numRows;
    uint16_t numColsB = pSrcB->numCols;
    uint16_t numColsA = pSrcA->numCols;
    float sumReal1, sumImag1;
    float a0, b0, c0, d0;
    float a1, b1, c1, d1;
    float sumReal2, sumImag2;
    uint16_t col, i = 0u, j, row = numRowsA, colCnt;
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
            px = pOut + 2 * i;
            col = numColsB;
            pIn2 = pSrcB->pData;
            j = 0u;
            do
            {
                sumReal1 = 0.0f;
                sumImag1 = 0.0f;
                sumReal2 = 0.0f;
                sumImag2 = 0.0f;
                pIn1 = pInA;
                colCnt = numColsA;
                while(colCnt > 0u)
                {
                    a1 = *pIn1;
                    c1 = *pIn2;
                    b1 = *(pIn1 + 1u);
                    d1 = *(pIn2 + 1u);
                    sumReal1 += a1 * c1;
                    sumImag1 += b1 * c1;
                    pIn1 += 2u;
                    pIn2 += 2 * numColsB;
                    sumReal2 -= b1 * d1;
                    sumImag2 += a1 * d1;
                    colCnt--;
                }
                sumReal1 += sumReal2;
                sumImag1 += sumImag2;
                *px++ = sumReal1;
                *px++ = sumImag1;
                j++;
                pIn2 = pSrcB->pData + 2u * j;
                col--;
            } while(col > 0u);
            i = i + numColsB;
            pInA = pInA + 2 * numColsA;
            row--;
        } while(row > 0u);
        status = ARM_MATH_SUCCESS;
    }
    return status;
}