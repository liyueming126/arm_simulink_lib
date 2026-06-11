/* Simplified implementation of ARM FIR filter for Simulink.
 * Based on CMSIS arm_fir_f32.c (Cortex-M0 version).
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

typedef struct {
    uint16_t numTaps;
    float32_t *pState;
    float32_t *pCoeffs;
} arm_fir_instance_f32;

void arm_fir_f32(
    const arm_fir_instance_f32 * S,
    float32_t * pSrc,
    float32_t * pDst,
    uint32_t blockSize)
{
    float32_t *pState = S->pState;
    float32_t *pCoeffs = S->pCoeffs;
    float32_t *pStateCurnt;
    float32_t *px, *pb;
    uint32_t numTaps = S->numTaps;
    uint32_t i, tapCnt, blkCnt;
    float32_t acc;

    pStateCurnt = &(S->pState[(numTaps - 1u)]);
    blkCnt = blockSize;

    while(blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc++;
        acc = 0.0f;
        px = pState;
        pb = pCoeffs;
        i = numTaps;
        do {
            acc += *px++ * *pb++;
            i--;
        } while(i > 0u);
        *pDst++ = acc;
        pState = pState + 1;
        blkCnt--;
    }

    pStateCurnt = S->pState;
    tapCnt = numTaps - 1u;
    while(tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;
        tapCnt--;
    }
}