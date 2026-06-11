
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint32_t numStages;
    q63_t *pState;
    q31_t *pCoeffs;
    uint8_t postShift;
} arm_biquad_cas_df1_32x64_ins_q31;

static q63_t mult32x64(q63_t x, q31_t y) {
    return (q63_t)((int64_t)(x >> 32) * (int64_t)y) + (q63_t)(((int64_t)((uint32_t)x) * (int64_t)y) >> 32);
}

void arm_biquad_cas_df1_32x64_q31(
    const arm_biquad_cas_df1_32x64_ins_q31 *S,
    q31_t *pSrc,
    q31_t *pDst,
    uint32_t blockSize)
{
    q31_t *pIn = pSrc;
    q31_t *pOut = pDst;
    q63_t *pState = S->pState;
    q31_t *pCoeffs = S->pCoeffs;
    q63_t acc;
    q31_t Xn1, Xn2;
    q63_t Yn1, Yn2;
    q31_t b0, b1, b2, a1, a2;
    q31_t Xn;
    int32_t shift = (int32_t)S->postShift + 1;
    uint32_t sample, stage = S->numStages;
    q31_t acc_l, acc_h;
    uint32_t uShift = ((uint32_t)S->postShift + 1u);
    uint32_t lShift = 32u - uShift;

    do {
        b0 = *pCoeffs++; b1 = *pCoeffs++; b2 = *pCoeffs++; a1 = *pCoeffs++; a2 = *pCoeffs++;
        Xn1 = (q31_t)(pState[0]); Xn2 = (q31_t)(pState[1]); Yn1 = pState[2]; Yn2 = pState[3];
        sample = blockSize;

        while (sample > 0u) {
            Xn = *pIn++;
            acc = (q63_t)Xn * b0; acc += (q63_t)Xn1 * b1; acc += (q63_t)Xn2 * b2;
            acc += mult32x64(Yn1, a1); acc += mult32x64(Yn2, a2);
            Xn2 = Xn1; Xn1 = Xn; Yn2 = Yn1; Yn1 = acc << shift;
            acc_l = acc & 0xffffffff; acc_h = (acc >> 32) & 0xffffffff;
            acc_h = (uint32_t)acc_l >> lShift | acc_h << uShift;
            *pOut++ = acc_h; sample--;
        }
        pIn = pDst; pOut = pDst;
        *pState++ = (q63_t)Xn1; *pState++ = (q63_t)Xn2; *pState++ = Yn1; *pState++ = Yn2;
    } while (--stage);
}
