#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    q15_t *pState;
    q15_t *pCoeffs;
    q15_t mu;
    uint32_t postShift;
} arm_lms_instance_q15;

static int16_t ssat16(int32_t val)
{
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (int16_t)val;
}

static void arm_lms_q15(
    const arm_lms_instance_q15 *S,
    q15_t *pSrc, q15_t *pRef, q15_t *pOut, q15_t *pErr,
    uint32_t blockSize)
{
    q15_t *pState = S->pState;
    uint32_t numTaps = S->numTaps;
    q15_t *pCoeffs = S->pCoeffs;
    q15_t *pStateCurnt;
    q15_t mu = S->mu;
    q15_t *px, *pb;
    uint32_t tapCnt, blkCnt;
    q63_t acc;
    q15_t e = 0;
    q15_t alpha;
    q31_t coef;
    q31_t acc_l, acc_h;
    int32_t lShift = (15 - (int32_t)S->postShift);
    int32_t uShift = (32 - lShift);

    pStateCurnt = &(S->pState[(numTaps - 1u)]);
    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc++;
        px = pState; pb = pCoeffs; acc = 0; tapCnt = numTaps;
        while (tapCnt > 0u) { acc += (q63_t)((q31_t)(*px++) * (*pb++)); tapCnt--; }
        acc_l = acc & 0xffffffff;
        acc_h = (acc >> 32) & 0xffffffff;
        acc = (uint32_t)acc_l >> lShift | acc_h << uShift;
        acc = ssat16((int32_t)acc);
        *pOut++ = (q15_t)acc;
        e = *pRef++ - (q15_t)acc; *pErr++ = (q15_t)e;
        alpha = (q15_t)(((q31_t)e * (mu)) >> 15);
        px = pState++; pb = pCoeffs; tapCnt = numTaps;
        while (tapCnt > 0u) { coef = (q31_t)*pb + (((q31_t)alpha * (*px++)) >> 15); *pb++ = ssat16(coef); tapCnt--; }
        blkCnt--;
    }

    pStateCurnt = S->pState;
    tapCnt = (numTaps - 1u);
    while (tapCnt > 0u) { *pStateCurnt++ = *pState++; tapCnt--; }
}
