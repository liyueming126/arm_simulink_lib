
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t; typedef int64_t q63_t;
typedef struct { uint8_t M; uint16_t numTaps; q31_t *pCoeffs; q31_t *pState; } arm_fir_decimate_instance_q31;

void arm_fir_decimate_fast_q31(arm_fir_decimate_instance_q31 *S, q31_t *pSrc, q31_t *pDst, uint32_t blockSize) {
    q31_t *pState = S->pState, *pCoeffs = S->pCoeffs, *pStateCurnt, x0, c0, *px, *pb, sum0;
    uint32_t numTaps = S->numTaps, i, tapCnt, blkCnt, outBlockSize = blockSize / S->M;
    pStateCurnt = S->pState + (numTaps - 1u); blkCnt = outBlockSize;
    while (blkCnt > 0u) { i = S->M; do { *pStateCurnt++ = *pSrc++; } while (--i);
        sum0 = 0; px = pState; pb = pCoeffs; tapCnt = numTaps;
        while (tapCnt > 0u) { c0 = *pb++; x0 = *px++; sum0 = (q31_t)((((q63_t)sum0 << 32) + ((q63_t)x0 * c0)) >> 32); tapCnt--; }
        pState = pState + S->M; *pDst++ = (q31_t)(sum0 << 1); blkCnt--; }
    pStateCurnt = S->pState; i = numTaps - 1u; while (i > 0u) { *pStateCurnt++ = *pState++; i--; }
}
