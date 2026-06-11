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
    uint8_t postShift;
    q15_t energy;
    q15_t x0;
    q15_t *recipTable;
} arm_lms_norm_instance_q15;

#define DELTA_Q15 ((q15_t)1)

static int16_t ssat16(int32_t val)
{
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (int16_t)val;
}

static q15_t arm_recip_q15(q15_t energy, q15_t *oneByEnergy, q15_t *recipTable)
{
    q15_t in;
    int16_t shift;
    int32_t temp;
    (void)recipTable;

    if (energy <= 0) { *oneByEnergy = 32767; return 0; }
    in = energy; shift = 0;
    while (in < 16384 && shift < 14) { in <<= 1; shift++; }
    if (shift > 0) shift--;
    temp = (int32_t)32767 * 32768;
    temp = temp / (int32_t)in;
    if (temp > 32767) temp = 32767;
    if (temp < 0) temp = 0;
    *oneByEnergy = (q15_t)temp;
    return (q15_t)shift;
}

static void arm_lms_norm_q15(
    arm_lms_norm_instance_q15 *S,
    q15_t *pSrc, q15_t *pRef, q15_t *pOut, q15_t *pErr,
    uint32_t blockSize)
{
    q15_t *pState = S->pState;
    q15_t *pCoeffs = S->pCoeffs;
    q15_t *pStateCurnt;
    q15_t *px, *pb;
    q15_t mu = S->mu;
    uint32_t numTaps = S->numTaps;
    uint32_t tapCnt, blkCnt;
    q31_t energy;
    q63_t acc;
    q15_t e = 0, d = 0;
    q15_t w = 0, in;
    q15_t x0;
    q15_t errorXmu, oneByEnergy;
    q15_t postShift;
    q31_t coef;
    q31_t acc_l, acc_h;
    int32_t lShift = (15 - (int32_t)S->postShift);
    int32_t uShift = (32 - lShift);

    energy = S->energy; x0 = S->x0;
    pStateCurnt = &(S->pState[(numTaps - 1u)]);
    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc;
        px = pState; pb = pCoeffs;
        in = *pSrc++;
        energy -= (((q31_t)x0 * (x0)) >> 15);
        energy += (((q31_t)in * (in)) >> 15);
        acc = 0; tapCnt = numTaps;
        while (tapCnt > 0u) { acc += (((q31_t)*px++ * (*pb++))); tapCnt--; }
        acc_l = acc & 0xffffffff; acc_h = (acc >> 32) & 0xffffffff;
        acc = (uint32_t)acc_l >> lShift | acc_h << uShift;
        acc = ssat16((int32_t)acc); *pOut++ = (q15_t)acc;
        d = *pRef++; e = d - (q15_t)acc; *pErr++ = e;
        postShift = arm_recip_q15((q15_t)energy + DELTA_Q15, &oneByEnergy, S->recipTable);
        errorXmu = (q15_t)(((q31_t)e * mu) >> 15);
        w = ssat16((int32_t)((((q31_t)errorXmu * oneByEnergy) >> (15 - postShift))));
        px = pState; pb = (pCoeffs); tapCnt = numTaps;
        while (tapCnt > 0u) { coef = *pb + (((q31_t)w * (*px++)) >> 15); *pb++ = ssat16(coef); tapCnt--; }
        x0 = *pState; pState = pState + 1u;
        blkCnt--;
    }

    S->energy = (q15_t)energy; S->x0 = x0;
    pStateCurnt = S->pState; tapCnt = (numTaps - 1u);
    while (tapCnt > 0u) { *pStateCurnt++ = *pState++; tapCnt--; }
}
