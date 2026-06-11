
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t; typedef int32_t q31_t;
typedef struct { uint16_t numStages; q15_t *pState; q15_t *pCoeffs; } arm_fir_lattice_instance_q15;

static q15_t ssat16(int32_t val) { if (val > 32767) return 32767; if (val < -32768) return -32768; return (q15_t)val; }

void arm_fir_lattice_q15(const arm_fir_lattice_instance_q15 *S, q15_t *pSrc, q15_t *pDst, uint32_t blockSize) {
    q31_t fcurnt, fnext, gcurnt, gnext; uint32_t numStages = S->numStages, blkCnt, stageCnt; q15_t *pState, *px, *pk;
    pState = &S->pState[0]; blkCnt = blockSize;
    while (blkCnt > 0u) { fcurnt = *pSrc++; pk = (S->pCoeffs); px = pState; gcurnt = *px;
        fnext = ((gcurnt * (*pk)) >> 15u) + fcurnt; fnext = ssat16(fnext);
        gnext = ((fcurnt * (*pk++)) >> 15u) + gcurnt; gnext = ssat16(gnext);
        *px++ = (q15_t)fcurnt; fcurnt = fnext; stageCnt = (numStages - 1u);
        while (stageCnt > 0u) { gcurnt = *px; *px++ = (q15_t)gnext;
            fnext = ((gcurnt * (*pk)) >> 15u) + fcurnt; fnext = ssat16(fnext);
            gnext = ((fcurnt * (*pk++)) >> 15u) + gcurnt; gnext = ssat16(gnext);
            fcurnt = fnext; stageCnt--; }
        *pDst++ = ssat16(fcurnt); blkCnt--; }
}
