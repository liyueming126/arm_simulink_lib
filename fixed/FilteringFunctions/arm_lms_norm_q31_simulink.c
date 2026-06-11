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
    uint8_t postShift;
    q31_t energy;
    q31_t x0;
    q31_t *recipTable;
} arm_lms_norm_instance_q31;

#define DELTA_Q31 ((q31_t)1)

static int32_t clip_q63_to_q31(int64_t x)
{
    if (x > 2147483647LL) return 2147483647L;
    if (x < -2147483648LL) return -2147483648L;
    return (int32_t)x;
}

static int32_t arm_recip_q31(q31_t energy, q31_t *oneByEnergy, q31_t *recipTable)
{
    q31_t in;
    int32_t shift;
    (void)recipTable;

    if (energy <= 0) { *oneByEnergy = 2147483647L; return 0; }
    in = energy; shift = 0;
    while (in < 1073741824 && shift < 30) { in <<= 1; shift++; }
    if (shift > 0) shift--;
    {
        int64_t temp = 2147483647LL * 2147483648LL;
        temp = temp / (int64_t)in;
        if (temp > 2147483647LL) temp = 2147483647LL;
        if (temp < 0) temp = 0;
        *oneByEnergy = (q31_t)temp;
    }
    return shift;
}

static void arm_lms_norm_q31(
    arm_lms_norm_instance_q31 *S,
    q31_t *pSrc, q31_t *pRef, q31_t *pOut, q31_t *pErr,
    uint32_t blockSize)
{
    q31_t *pState = S->pState;
    q31_t *pCoeffs = S->pCoeffs;
    q31_t *pStateCurnt;
    q31_t *px, *pb;
    q31_t mu = S->mu;
    uint32_t numTaps = S->numTaps;
    uint32_t tapCnt, blkCnt;
    q63_t energy;
    q63_t acc;
    q31_t e = 0, d = 0;
    q31_t w = 0, in;
    q31_t x0;
    q31_t errorXmu, oneByEnergy;
    q31_t postShift;
    q31_t coef;
    q31_t acc_l, acc_h;
    uint32_t uShift = ((uint32_t)S->postShift + 1u);
    uint32_t lShift = 32u - uShift;

    energy = S->energy; x0 = S->x0;
    pStateCurnt = &(S->pState[(numTaps - 1u)]);
    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc;
        px = pState; pb = pCoeffs; in = *pSrc++;
        energy = (q31_t)((((q63_t)energy << 32) - (((q63_t)x0 * x0) << 1)) >> 32);
        energy = (q31_t)(((((q63_t)in * in) << 1) + (energy << 32)) >> 32);
        acc = 0; tapCnt = numTaps;
        while (tapCnt > 0u) { acc += ((q63_t)(*px++)) * (*pb++); tapCnt--; }
        acc_l = acc & 0xffffffff; acc_h = (acc >> 32) & 0xffffffff;
        acc = (uint32_t)acc_l >> lShift | acc_h << uShift;
        *pOut++ = (q31_t)acc;
        d = *pRef++; e = d - (q31_t)acc; *pErr++ = e;
        postShift = arm_recip_q31(energy + DELTA_Q31, &oneByEnergy, &S->recipTable[0]);
        errorXmu = (q31_t)(((q63_t)e * mu) >> 31);
        w = clip_q63_to_q31(((q63_t)errorXmu * oneByEnergy) >> (31 - postShift));
        px = pState; pb = (pCoeffs); tapCnt = numTaps;
        while (tapCnt > 0u) { coef = (q31_t)(((q63_t)w * (*px++)) >> (32)); *pb = clip_q63_to_q31((q63_t)*pb + (coef << 1u)); pb++; tapCnt--; }
        x0 = *pState; pState = pState + 1;
        blkCnt--;
    }

    S->energy = (q31_t)energy; S->x0 = x0;
    pStateCurnt = S->pState; tapCnt = (numTaps - 1u);
    while (tapCnt > 0u) { *pStateCurnt++ = *pState++; tapCnt--; }
}
