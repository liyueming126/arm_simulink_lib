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

arm_status arm_mat_sub_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
  arm_matrix_instance_f32 * pDst)
{
    float *pIn1 = pSrcA->pData;
    float *pIn2 = pSrcB->pData;
    float *pOut = pDst->pData;
    uint32_t numSamples;
    uint32_t blkCnt;
    arm_status status;

    if((pSrcA->numRows != pSrcB->numRows) ||
       (pSrcA->numCols != pSrcB->numCols) ||
       (pSrcA->numRows != pDst->numRows) || (pSrcA->numCols != pDst->numCols))
    {
        status = ARM_MATH_SIZE_MISMATCH;
    }
    else
    {
        numSamples = (uint32_t) pSrcA->numRows * pSrcA->numCols;
        blkCnt = numSamples;
        while(blkCnt > 0u)
        {
            *pOut++ = (*pIn1++) - (*pIn2++);
            blkCnt--;
        }
        status = ARM_MATH_SUCCESS;
    }
    return status;
}