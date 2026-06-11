/* Simplified implementation of ARM convolution for Simulink.
 * Based on CMSIS arm_conv_f32.c (Cortex-M0 version).
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;

void arm_conv_f32(
    float32_t * pSrcA,
    uint32_t srcALen,
    float32_t * pSrcB,
    uint32_t srcBLen,
    float32_t * pDst)
{
    float32_t *pIn1 = pSrcA;
    float32_t *pIn2 = pSrcB;
    float32_t sum;
    uint32_t i, j;

    for (i = 0u; i < ((srcALen + srcBLen) - 1u); i++)
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
}