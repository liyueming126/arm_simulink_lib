
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t; typedef int64_t q63_t;
typedef struct { uint16_t numStages; q31_t *pState; q31_t *pCoeffs; } arm_fir_lattice_instance_q31;

void arm_fir_lattice_q31(const arm_fir_lattice_instance_q31 *S, q31_t *pSrc, q31_t *pDst, uint32_t blockSize) {
    q31_t *pState, *px, *pk, fcurr, fnext, gcurr, gnext; uint32_t numStages = S->numStages, blkCnt, stageCnt;
    pState = &S->pState[0]; blkCnt = blockSize;
    while (blkCnt > 0u) { fcurr = *pSrc++; pk = (S->pCoeffs); px = pState; gcurr = *px;
        fnext = (q31_t)(((q63_t)gcurr * (*pk)) >> 31) + fcurr; gnext = (q31_t)(((q63_t)fcurr * (*pk++)) >> 31) + gcurr;
        *px++ = fcurr; fcurr = fnext; stageCnt = (numStages - 1u);
        while (stageCnt > 0u) { gcurr = *px; *px++ = gnext; fnext = (q31_t)(((q63_t)gcurr * (*pk)) >> 31) + fcurr; gnext = (q31_t)(((q63_t)fcurr * (*pk++)) >> 31) + gcurr; fcurr = fnext; stageCnt--; }
        *pDst++ = fcurr; blkCnt--; }
}
