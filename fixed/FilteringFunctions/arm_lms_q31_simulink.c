#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    q31_t *pState;
    q31_t *pCoeffs;
    q31_t mu;
    uint32_t postShift;
} arm_lms_instance_q31;

static int32_t clip_q63_to_q31(int64_t x)
{
    if (x > 2147483647LL) return 2147483647L;
    if (x < -2147483648LL) return -2147483648L;
    return (int32_t)x;
}

static void arm_lms_q31(
    const arm_lms_instance_q31 *S,
    q31_t *pSrc, q31_t *pRef, q31_t *pOut, q31_t *pErr,
    uint32_t blockSize)
{
    q31_t *pState = S->pState;
    uint32_t numTaps = S->numTaps;
    q31_t *pCoeffs = S->pCoeffs;
    q31_t *pStateCurnt;
    q31_t mu = S->mu;
    q31_t *px, *pb;
    uint32_t tapCnt, blkCnt;
    q63_t acc;
    q31_t e = 0, alpha, coef;
    q31_t acc_l, acc_h;
    uint32_t uShift = ((uint32_t)S->postShift + 1u);
    uint32_t lShift = 32u - uShift;

    pStateCurnt = &(S->pState[(numTaps - 1u)]);
    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc++;
        px = pState; pb = pCoeffs; acc = 0; tapCnt = numTaps;
        while (tapCnt > 0u) { acc += ((q63_t)(*px++)) * (*pb++); tapCnt--; }
        acc_l = acc & 0xffffffff;
        acc_h = (acc >> 32) & 0xffffffff;
        acc = (uint32_t)acc_l >> lShift | acc_h << uShift;
        *pOut++ = (q31_t)acc;
        e = *pRef++ - (q31_t)acc; *pErr++ = (q31_t)e;
        alpha = (q31_t)(((q63_t)e * mu) >> 31);
        px = pState++; pb = pCoeffs; tapCnt = numTaps;
        while (tapCnt > 0u) { coef = (q31_t)(((q63_t)alpha * (*px++)) >> (32)); *pb = clip_q63_to_q31((q63_t)*pb + (coef << 1u)); pb++; tapCnt--; }
        blkCnt--;
    }

    pStateCurnt = S->pState;
    tapCnt = (numTaps - 1u);
    while (tapCnt > 0u) { *pStateCurnt++ = *pState++; tapCnt--; }
}
