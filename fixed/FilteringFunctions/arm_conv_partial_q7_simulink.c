#include <stdint.h>

typedef int8_t q7_t;
typedef int32_t arm_status;
#define ARM_MATH_SUCCESS 0
#define ARM_MATH_ARGUMENT_ERROR 1

static arm_status arm_conv_partial_q7(
    q7_t *pSrcA,
    uint32_t srcALen,
    q7_t *pSrcB,
    uint32_t srcBLen,
    q7_t *pDst,
    uint32_t firstIndex,
    uint32_t numPoints)
{
    int32_t sum;
    uint32_t i, j;
    arm_status status;

    if ((firstIndex + numPoints) > (srcALen + (srcBLen - 1u)))
    {
        status = ARM_MATH_ARGUMENT_ERROR;
    }
    else
    {
        for (i = firstIndex; i < (firstIndex + numPoints); i++)
        {
            sum = 0;

            for (j = 0; j <= i; j++)
            {
                if (((i - j) < srcBLen) && (j < srcALen))
                {
                    sum += (int16_t)pSrcA[j] * (int16_t)pSrcB[i - j];
                }
            }

            sum = sum >> 7;
            if (sum > 127) sum = 127;
            if (sum < -128) sum = -128;
            *pDst++ = (q7_t)sum;
        }
        status = ARM_MATH_SUCCESS;
    }

    return status;
}
