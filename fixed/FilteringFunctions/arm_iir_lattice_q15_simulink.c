
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t; typedef int32_t q31_t; typedef int64_t q63_t;
typedef struct { uint16_t numStages; q15_t *pState; q15_t *pkCoeffs; q15_t *pvCoeffs; } arm_iir_lattice_instance_q15;

static q15_t ssat16(int32_t val) { if (val > 32767) return 32767; if (val < -32768) return -32768; return (q15_t)val; }

void arm_iir_lattice_q15(const arm_iir_lattice_instance_q15 *S, q15_t *pSrc, q15_t *pDst, uint32_t blockSize) {
    q31_t fcurr, fnext = 0, gcurr = 0, gnext; uint32_t stgCnt; q63_t acc; uint32_t blkCnt, tapCnt;
    q15_t *px1, *px2, *pk, *pv; uint32_t numStages = S->numStages; q15_t *pState, *pStateCurnt, out;
    blkCnt = blockSize; pState = &S->pState[0];
    while (blkCnt > 0u) { fcurr = *pSrc++; px1 = pState; px2 = pState; acc = 0; pv = &S->pvCoeffs[0]; pk = &S->pkCoeffs[0]; tapCnt = numStages;
        while (tapCnt > 0u) { gcurr = *px1++; fnext = fcurr - ((gcurr * (*pk)) >> 15); fnext = ssat16(fnext); gnext = ((fnext * (*pk++)) >> 15) + gcurr; gnext = ssat16(gnext); acc += (q31_t)((gnext * (*pv++))); *px2++ = (q15_t)gnext; fcurr = fnext; tapCnt--; }
        acc += (q31_t)((fnext * (*pv++))); out = (q15_t)ssat16(acc >> 15); *px2++ = (q15_t)fnext; *pDst++ = out; pState = pState + 1u; blkCnt--; }
    pStateCurnt = &S->pState[0]; pState = &S->pState[blockSize]; stgCnt = numStages;
    while (stgCnt > 0u) { *pStateCurnt++ = *pState++; stgCnt--; }
}
