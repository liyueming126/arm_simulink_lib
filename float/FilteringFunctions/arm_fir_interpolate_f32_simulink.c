/* Simplified implementation of ARM FIR interpolate for Simulink.
 * Based on CMSIS arm_fir_interpolate_f32.c (Cortex-M0 version).
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef struct {
    uint8_t L;
    uint16_t phaseLength;
    float32_t *pCoeffs;
    float32_t *pState;
} arm_fir_interpolate_instance_f32;

void arm_fir_interpolate_f32(
    const arm_fir_interpolate_instance_f32 * S,
    float32_t * pSrc,
    float32_t * pDst,
    uint32_t blockSize)
{
    float32_t *pState = S->pState;
    float32_t *pCoeffs = S->pCoeffs;
    float32_t *pStateCurnt;
    float32_t *ptr1, *ptr2;
    float32_t sum;
    uint32_t i, blkCnt;
    uint16_t phaseLen = S->phaseLength, tapCnt;

    pStateCurnt = S->pState + (phaseLen - 1u);
    blkCnt = blockSize;

    while(blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc++;
        i = S->L;
        while(i > 0u)
        {
            sum = 0.0f;
            ptr1 = pState;
            ptr2 = pCoeffs + (i - 1u);
            tapCnt = phaseLen;
            while(tapCnt > 0u)
            {
                sum += *ptr1++ * *ptr2;
                ptr2 += S->L;
                tapCnt--;
            }
            *pDst++ = sum;
            i--;
        }
        pState = pState + 1;
        blkCnt--;
    }

    pStateCurnt = S->pState;
    tapCnt = phaseLen - 1u;
    while(tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;
        tapCnt--;
    }
}