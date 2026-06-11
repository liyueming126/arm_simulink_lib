#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef int64_t q63_t;
typedef int32_t arm_status;
#define ARM_MATH_SUCCESS 0
#define ARM_MATH_ARGUMENT_ERROR 1

static arm_status arm_conv_partial_fast_q31(
    q31_t *pSrcA,
    uint32_t srcALen,
    q31_t *pSrcB,
    uint32_t srcBLen,
    q31_t *pDst,
    uint32_t firstIndex,
    uint32_t numPoints)
{
    q31_t sum;
    uint32_t i, j;

    if ((firstIndex + numPoints) > (srcALen + srcBLen - 1u))
    {
        return ARM_MATH_ARGUMENT_ERROR;
    }

    for (i = firstIndex; i < firstIndex + numPoints; i++)
    {
        sum = 0;

        for (j = 0; j <= i; j++)
        {
            if (((i - j) < srcBLen) && (j < srcALen))
            {
                sum = (q31_t)((((q63_t)sum << 32) + ((q63_t)pSrcA[j] * pSrcB[i - j])) >> 32);
            }
        }

        pDst[i] = (q31_t)(sum << 1);
    }

    return ARM_MATH_SUCCESS;
}
