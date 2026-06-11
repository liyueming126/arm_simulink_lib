/* Simplified implementation of ARM LMS filter for Simulink.
 * Based on CMSIS arm_lms_f32.c (Cortex-M0 version).
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
    float32_t mu;
} arm_lms_instance_f32;

void arm_lms_f32(
    const arm_lms_instance_f32 * S,
    float32_t * pSrc,
    float32_t * pRef,
    float32_t * pOut,
    float32_t * pErr,
    uint32_t blockSize)
{
    float32_t *pState = S->pState;
    float32_t *pCoeffs = S->pCoeffs;
    float32_t *pStateCurnt;
    float32_t *px, *pb;
    float32_t mu = S->mu;
    uint32_t numTaps = S->numTaps;
    uint32_t tapCnt, blkCnt;
    float32_t sum, e, d;
    float32_t w = 0.0f;

    e = 0.0f;
    d = 0.0f;

    pStateCurnt = &(S->pState[(numTaps - 1u)]);
    blkCnt = blockSize;

    while(blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc++;
        px = pState;
        pb = pCoeffs;
        sum = 0.0f;

        tapCnt = numTaps;
        while(tapCnt > 0u)
        {
            sum += (*px++) * (*pb++);
            tapCnt--;
        }

        *pOut++ = sum;
        d = (float32_t)(*pRef++);
        e = d - sum;
        *pErr++ = e;

        w = e * mu;
        px = pState;
        pb = pCoeffs;

        tapCnt = numTaps;
        while(tapCnt > 0u)
        {
            *pb = *pb + (w * (*px++));
            pb++;
            tapCnt--;
        }

        pState = pState + 1;
        blkCnt--;
    }

    pStateCurnt = S->pState;
    tapCnt = (numTaps - 1u);
    while(tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;
        tapCnt--;
    }
}