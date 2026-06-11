/* Simplified implementation of ARM FIR lattice filter for Simulink.
 * Based on CMSIS arm_fir_lattice_f32.c (Cortex-M0 version).
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

typedef struct {
    uint16_t numStages;
    float32_t *pState;
    float32_t *pCoeffs;
} arm_fir_lattice_instance_f32;

void arm_fir_lattice_f32(
    const arm_fir_lattice_instance_f32 * S,
    float32_t * pSrc,
    float32_t * pDst,
    uint32_t blockSize)
{
    float32_t *pState;
    float32_t *pCoeffs = S->pCoeffs;
    float32_t *px;
    float32_t *pk;
    float32_t fcurr, fnext, gcurr, gnext;
    uint32_t numStages = S->numStages;
    uint32_t blkCnt, stageCnt;

    pState = &S->pState[0];
    blkCnt = blockSize;

    while(blkCnt > 0u)
    {
        fcurr = *pSrc++;
        pk = pCoeffs;
        px = pState;
        gcurr = *px;
        fnext = fcurr + ((*pk) * gcurr);
        gnext = (fcurr * (*pk++)) + gcurr;
        *px++ = fcurr;
        fcurr = fnext;
        stageCnt = (numStages - 1u);
        while(stageCnt > 0u)
        {
            gcurr = *px;
            *px++ = gnext;
            fnext = fcurr + ((*pk) * gcurr);
            gnext = (fcurr * (*pk++)) + gcurr;
            fcurr = fnext;
            stageCnt--;
        }
        *pDst++ = fcurr;
        blkCnt--;
    }
}