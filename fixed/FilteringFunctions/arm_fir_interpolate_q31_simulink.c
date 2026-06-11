
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t; typedef int64_t q63_t;
typedef struct { uint8_t L; uint16_t phaseLength; q31_t *pCoeffs; q31_t *pState; } arm_fir_interpolate_instance_q31;

void arm_fir_interpolate_q31(const arm_fir_interpolate_instance_q31 *S, q31_t *pSrc, q31_t *pDst, uint32_t blockSize) {
    q31_t *pState = S->pState, *pCoeffs = S->pCoeffs, *pStateCurnt, *ptr1, *ptr2;
    q63_t sum; q31_t x0, c0; uint32_t i, blkCnt; uint16_t phaseLen = S->phaseLength, tapCnt;
    pStateCurnt = S->pState + ((q31_t)phaseLen - 1); blkCnt = blockSize;
    while (blkCnt > 0u) { *pStateCurnt++ = *pSrc++; i = S->L;
        while (i > 0u) { sum = 0; ptr1 = pState; ptr2 = pCoeffs + (i - 1u); tapCnt = phaseLen;
            while (tapCnt > 0u) { c0 = *(ptr2); ptr2 += S->L; x0 = *ptr1++; sum += (q63_t)x0 * c0; tapCnt--; }
            *pDst++ = (q31_t)(sum >> 31); i--; }
        pState = pState + 1; blkCnt--; }
    pStateCurnt = S->pState; tapCnt = phaseLen - 1u; while (tapCnt > 0u) { *pStateCurnt++ = *pState++; tapCnt--; }
}
