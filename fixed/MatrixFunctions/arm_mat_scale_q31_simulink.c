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

arm_status arm_mat_scale_q31(
    const arm_matrix_instance_q31 *pSrc,
    q31_t scaleFract,
    int32_t shift,
    arm_matrix_instance_q31 *pDst)
{
    q31_t *pIn = pSrc->pData;
    q31_t *pOut = pDst->pData;
    uint32_t numSamples;
    uint32_t blkCnt;
    q31_t in1, in2, out1;
    int32_t totShift;

    if ((pSrc->numRows != pDst->numRows) || (pSrc->numCols != pDst->numCols))
        return ARM_MATH_SIZE_MISMATCH;

    numSamples = (uint32_t)pSrc->numRows * pSrc->numCols;
    totShift = shift + 1;
    blkCnt = numSamples;

    while (blkCnt > 0u)
    {
        in1 = *pIn++;
        in2 = (q31_t)(((q63_t)in1 * (q63_t)scaleFract) >> 32);
        out1 = in2 << totShift;

        if (in2 != (out1 >> totShift))
        {
            if (in2 < 0)
                out1 = -2147483648;
            else
                out1 = 2147483647;
        }

        *pOut++ = out1;
        blkCnt--;
    }

    return ARM_MATH_SUCCESS;
}
