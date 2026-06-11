/* Simplified implementation of ARM IIR lattice filter for Simulink.
 * Based on CMSIS arm_iir_lattice_f32.c (Cortex-M0 version).
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

typedef struct {
    uint16_t numStages;
    float32_t *pState;
    float32_t *pkCoeffs;
    float32_t *pvCoeffs;
} arm_iir_lattice_instance_f32;

void arm_iir_lattice_f32(
    const arm_iir_lattice_instance_f32 * S,
    float32_t * pSrc,
    float32_t * pDst,
    uint32_t blockSize)
{
    float32_t fcurr, fnext = 0, gcurr, gnext;
    float32_t acc;
    uint32_t blkCnt, tapCnt;
    float32_t *px1, *px2, *pk, *pv;
    uint32_t numStages = S->numStages;
    float32_t *pState;
    float32_t *pStateCurnt;

    blkCnt = blockSize;
    pState = &S->pState[0];

    while(blkCnt > 0u)
    {
        fcurr = *pSrc++;
        px1 = pState;
        px2 = pState;
        acc = 0.0f;
        pv = &S->pvCoeffs[0];
        pk = &S->pkCoeffs[0];

        tapCnt = numStages;
        while(tapCnt > 0u)
        {
            gcurr = *px1++;
            fnext = fcurr - ((*pk) * gcurr);
            gnext = (fnext * (*pk++)) + gcurr;
            acc += (gnext * (*pv++));
            *px2++ = gnext;
            fcurr = fnext;
            tapCnt--;
        }

        acc += (fnext * (*pv));
        *px2++ = fnext;
        *pDst++ = acc;
        pState = pState + 1u;
        blkCnt--;
    }

    pStateCurnt = &S->pState[0];
    pState = &S->pState[blockSize];
    tapCnt = numStages;
    while(tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;
        tapCnt--;
    }
}