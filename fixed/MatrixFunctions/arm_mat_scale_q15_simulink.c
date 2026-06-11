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

arm_status arm_mat_scale_q15(
    const arm_matrix_instance_q15 *pSrc,
    q15_t scaleFract,
    int32_t shift,
    arm_matrix_instance_q15 *pDst)
{
    q15_t *pIn = pSrc->pData;
    q15_t *pOut = pDst->pData;
    uint32_t numSamples;
    uint32_t blkCnt;
    int32_t val;
    int32_t totShift;

    if ((pSrc->numRows != pDst->numRows) || (pSrc->numCols != pDst->numCols))
        return ARM_MATH_SIZE_MISMATCH;

    numSamples = (uint32_t)pSrc->numRows * pSrc->numCols;
    totShift = 15 - shift;
    blkCnt = numSamples;

    while (blkCnt > 0u)
    {
        val = ((int32_t)*pIn++ * (int32_t)scaleFract) >> totShift;

        if (val > 32767)
            val = 32767;
        else if (val < -32768)
            val = -32768;

        *pOut++ = (q15_t)val;
        blkCnt--;
    }

    return ARM_MATH_SUCCESS;
}
