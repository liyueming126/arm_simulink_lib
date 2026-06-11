
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t; typedef int32_t q31_t; typedef int64_t q63_t;
typedef struct { uint8_t L; uint16_t phaseLength; q15_t *pCoeffs; q15_t *pState; } arm_fir_interpolate_instance_q15;

static q15_t ssat16(int32_t val) { if (val > 32767) return 32767; if (val < -32768) return -32768; return (q15_t)val; }

void arm_fir_interpolate_q15(const arm_fir_interpolate_instance_q15 *S, q15_t *pSrc, q15_t *pDst, uint32_t blockSize) {
    q15_t *pState = S->pState, *pCoeffs = S->pCoeffs, *pStateCurnt, *ptr1, *ptr2;
    q63_t sum; q15_t x0, c0; uint32_t i, blkCnt, tapCnt; uint16_t phaseLen = S->phaseLength;
    pStateCurnt = S->pState + (phaseLen - 1u); blkCnt = blockSize;
    while (blkCnt > 0u) { *pStateCurnt++ = *pSrc++; i = S->L;
        while (i > 0u) { sum = 0; ptr1 = pState; ptr2 = pCoeffs + (i - 1u); tapCnt = (uint32_t)phaseLen;
            while (tapCnt > 0u) { c0 = *ptr2; ptr2 += S->L; x0 = *ptr1++; sum += ((q31_t)x0 * c0); tapCnt--; }
            *pDst++ = (q15_t)(ssat16((sum >> 15))); i--; }
        pState = pState + 1; blkCnt--; }
    pStateCurnt = S->pState; i = (uint32_t)phaseLen - 1u;
    while (i > 0u) { *pStateCurnt++ = *pState++; i--; }
}
