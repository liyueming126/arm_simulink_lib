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

arm_status arm_mat_scale_f32(
  const arm_matrix_instance_f32 * pSrc,
  float32_t scale,
  arm_matrix_instance_f32 * pDst)
{
    float *pIn = pSrc->pData;
    float *pOut = pDst->pData;
    uint32_t numSamples;
    uint32_t blkCnt;
    arm_status status;

    if((pSrc->numRows != pDst->numRows) || (pSrc->numCols != pDst->numCols))
    {
        status = ARM_MATH_SIZE_MISMATCH;
    }
    else
    {
        numSamples = (uint32_t) pSrc->numRows * pSrc->numCols;
        blkCnt = numSamples;
        while(blkCnt > 0u)
        {
            *pOut++ = (*pIn++) * scale;
            blkCnt--;
        }
        status = ARM_MATH_SUCCESS;
    }
    return status;
}