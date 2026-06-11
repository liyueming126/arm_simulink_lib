
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int32_t q31_t;

typedef struct {
    uint8_t M; uint16_t numTaps; q15_t *pCoeffs; q15_t *pState;
} arm_fir_decimate_instance_q15;

static q15_t ssat16(int32_t val) {
    if (val > 32767) return 32767; if (val < -32768) return -32768; return (q15_t)val;
}

void arm_fir_decimate_fast_q15(
    const arm_fir_decimate_instance_q15 *S,
    q15_t *pSrc, q15_t *pDst, uint32_t blockSize)
{
    q15_t *pState = S->pState; q15_t *pCoeffs = S->pCoeffs;
    q15_t *pStateCurnt, *px, *pb; q15_t x0, c0; q31_t sum0;
    uint32_t numTaps = S->numTaps;
    uint32_t i, blkCnt, tapCnt, outBlockSize = blockSize / S->M;

    pStateCurnt = S->pState + (numTaps - 1u); blkCnt = outBlockSize;
    while (blkCnt > 0u) {
        i = S->M; do { *pStateCurnt++ = *pSrc++; } while (--i);
        sum0 = 0; px = pState; pb = pCoeffs; tapCnt = numTaps;
        while (tapCnt > 0u) { c0 = *pb++; x0 = *px++; sum0 += x0 * c0; tapCnt--; }
        pState = pState + S->M; *pDst++ = (q15_t)(ssat16((sum0 >> 15))); blkCnt--;
    }
    pStateCurnt = S->pState; i = numTaps - 1u;
    while (i > 0u) { *pStateCurnt++ = *pState++; i--; }
}
