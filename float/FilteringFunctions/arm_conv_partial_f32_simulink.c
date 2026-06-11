/* Simplified implementation of ARM partial convolution for Simulink.
 * Based on CMSIS arm_conv_partial_f32.c (Cortex-M0 version).
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef int arm_status;

#ifndef ARM_MATH_SUCCESS
#define ARM_MATH_SUCCESS 0
#endif
#ifndef ARM_MATH_ARGUMENT_ERROR
#define ARM_MATH_ARGUMENT_ERROR -2
#endif

arm_status arm_conv_partial_f32(
    float32_t * pSrcA,
    uint32_t srcALen,
    float32_t * pSrcB,
    uint32_t srcBLen,
    float32_t * pDst,
    uint32_t firstIndex,
    uint32_t numPoints)
{
    float32_t *pIn1 = pSrcA;
    float32_t *pIn2 = pSrcB;
    float32_t sum;
    uint32_t i, j;
    arm_status status;

    if ((firstIndex + numPoints) > ((srcALen + (srcBLen - 1u)))) {
        return ARM_MATH_ARGUMENT_ERROR;
    }

    for (i = firstIndex; i <= (firstIndex + numPoints - 1); i++)
    {
        sum = 0.0f;
        for (j = 0u; j <= i; j++)
        {
            if (((i - j) < srcBLen) && (j < srcALen))
            {
                sum += pIn1[j] * pIn2[i - j];
            }
        }
        pDst[i] = sum;
    }
    return ARM_MATH_SUCCESS;
}