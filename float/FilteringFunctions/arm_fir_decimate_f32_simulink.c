/* Simplified implementation of ARM FIR decimate for Simulink.
 * Based on CMSIS arm_fir_decimate_f32.c (Cortex-M0 version).
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef struct {
    uint8_t M;
    uint16_t numTaps;
    float32_t *pCoeffs;
    float32_t *pState;
} arm_fir_decimate_instance_f32;

void arm_fir_decimate_f32(
    const arm_fir_decimate_instance_f32 * S,
    float32_t * pSrc,
    float32_t * pDst,
    uint32_t blockSize)
{
    float32_t *pState = S->pState;
    float32_t *pCoeffs = S->pCoeffs;
    float32_t *pStateCurnt;
    float32_t *px, *pb;
    float32_t sum0;
    float32_t x0, c0;
    uint32_t numTaps = S->numTaps;
    uint32_t i, tapCnt, blkCnt, outBlockSize = blockSize / S->M;

    pStateCurnt = S->pState + (numTaps - 1u);
    blkCnt = outBlockSize;

    while(blkCnt > 0u)
    {
        i = S->M;
        do {
            *pStateCurnt++ = *pSrc++;
        } while(--i);

        sum0 = 0.0f;
        px = pState;
        pb = pCoeffs;
        tapCnt = numTaps;

        while(tapCnt > 0u)
        {
            c0 = *pb++;
            x0 = *px++;
            sum0 += x0 * c0;
            tapCnt--;
        }

        pState = pState + S->M;
        *pDst++ = sum0;
        blkCnt--;
    }

    pStateCurnt = S->pState;
    i = (numTaps - 1u);
    while(i > 0u)
    {
        *pStateCurnt++ = *pState++;
        i--;
    }
}