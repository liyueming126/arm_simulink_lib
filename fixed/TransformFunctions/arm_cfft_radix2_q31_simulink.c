#include <stdint.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t fftLen;
    uint8_t  ifftFlag;
    uint8_t  bitReverseFlag;
    uint16_t twidCoefModifier;
    uint16_t bitRevFactor;
    q31_t   *pTwiddle;
    uint16_t *pBitRevTable;
} arm_cfft_radix2_instance_q31;

void arm_bitreversal_q31(q31_t *pSrc, uint32_t fftLen, uint16_t bitRevFactor, uint16_t *pBitRevTable)
{
    uint32_t fftLenBy2, fftLenBy2p1, i, j;
    q31_t in;
    j = 0u;
    fftLenBy2 = fftLen / 2u;
    fftLenBy2p1 = (fftLen / 2u) + 1u;
    for (i = 0u; i <= (fftLenBy2 - 2u); i += 2u) {
        if (i < j) {
            in = pSrc[2u * i]; pSrc[2u * i] = pSrc[2u * j]; pSrc[2u * j] = in;
            in = pSrc[(2u * i) + 1u]; pSrc[(2u * i) + 1u] = pSrc[(2u * j) + 1u]; pSrc[(2u * j) + 1u] = in;
            in = pSrc[2u * (i + fftLenBy2p1)]; pSrc[2u * (i + fftLenBy2p1)] = pSrc[2u * (j + fftLenBy2p1)]; pSrc[2u * (j + fftLenBy2p1)] = in;
            in = pSrc[(2u * (i + fftLenBy2p1)) + 1u]; pSrc[(2u * (i + fftLenBy2p1)) + 1u] = pSrc[(2u * (j + fftLenBy2p1)) + 1u]; pSrc[(2u * (j + fftLenBy2p1)) + 1u] = in;
        }
        in = pSrc[2u * (i + 1u)]; pSrc[2u * (i + 1u)] = pSrc[2u * (j + fftLenBy2)]; pSrc[2u * (j + fftLenBy2)] = in;
        in = pSrc[(2u * (i + 1u)) + 1u]; pSrc[(2u * (i + 1u)) + 1u] = pSrc[(2u * (j + fftLenBy2)) + 1u]; pSrc[(2u * (j + fftLenBy2)) + 1u] = in;
        j = *pBitRevTable;
        pBitRevTable += bitRevFactor;
    }
}

void arm_radix2_butterfly_q31(q31_t *pSrc, uint32_t fftLen, q31_t *pCoef, uint16_t twidCoefModifier);
void arm_radix2_butterfly_inverse_q31(q31_t *pSrc, uint32_t fftLen, q31_t *pCoef, uint16_t twidCoefModifier);

void arm_cfft_radix2_q31(const arm_cfft_radix2_instance_q31 *S, q31_t *pSrc)
{
    if (S->ifftFlag == 1u)
        arm_radix2_butterfly_inverse_q31(pSrc, S->fftLen, S->pTwiddle, S->twidCoefModifier);
    else
        arm_radix2_butterfly_q31(pSrc, S->fftLen, S->pTwiddle, S->twidCoefModifier);
    arm_bitreversal_q31(pSrc, S->fftLen, S->bitRevFactor, S->pBitRevTable);
}

void arm_radix2_butterfly_q31(q31_t *pSrc, uint32_t fftLen, q31_t *pCoef, uint16_t twidCoefModifier)
{
    unsigned i, j, k, l, m;
    unsigned n1, n2, ia;
    q31_t xt, yt, cosVal, sinVal;
    q31_t p0, p1;

    n2 = fftLen;
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    for (i = 0; i < n2; i++) {
        cosVal = pCoef[ia * 2];
        sinVal = pCoef[(ia * 2) + 1];
        ia = ia + twidCoefModifier;
        l = i + n2;
        xt = (pSrc[2 * i] >> 1u) - (pSrc[2 * l] >> 1u);
        pSrc[2 * i] = ((pSrc[2 * i] >> 1u) + (pSrc[2 * l] >> 1u)) >> 1u;
        yt = (pSrc[2 * i + 1] >> 1u) - (pSrc[2 * l + 1] >> 1u);
        pSrc[2 * i + 1] = ((pSrc[2 * l + 1] >> 1u) + (pSrc[2 * i + 1] >> 1u)) >> 1u;
        p0 = (q31_t)(((q63_t)xt * cosVal) >> 32);
        p1 = (q31_t)(((q63_t)yt * cosVal) >> 32);
        p0 += (q31_t)(((q63_t)yt * sinVal) >> 32);
        p1 -= (q31_t)(((q63_t)xt * sinVal) >> 32);
        pSrc[2u * l] = p0;
        pSrc[2u * l + 1u] = p1;
    }

    twidCoefModifier <<= 1u;

    for (k = fftLen / 2; k > 2; k = k >> 1) {
        n1 = n2;
        n2 = n2 >> 1;
        ia = 0;
        for (j = 0; j < n2; j++) {
            cosVal = pCoef[ia * 2];
            sinVal = pCoef[(ia * 2) + 1];
            ia = ia + twidCoefModifier;
            i = j;
            m = fftLen / n1;
            do {
                l = i + n2;
                xt = pSrc[2 * i] - pSrc[2 * l];
                pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]) >> 1u;
                yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
                pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]) >> 1u;
                p0 = (q31_t)(((q63_t)xt * cosVal) >> 32);
                p1 = (q31_t)(((q63_t)yt * cosVal) >> 32);
                p0 += (q31_t)(((q63_t)yt * sinVal) >> 32);
                p1 -= (q31_t)(((q63_t)xt * sinVal) >> 32);
                pSrc[2u * l] = p0;
                pSrc[2u * l + 1u] = p1;
                i += n1;
                m--;
            } while(m > 0);
        }
        twidCoefModifier <<= 1u;
    }

    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;
    cosVal = pCoef[ia * 2];
    sinVal = pCoef[(ia * 2) + 1];
    ia = ia + twidCoefModifier;

    for (i = 0; i < fftLen; i += n1) {
        l = i + n2;
        xt = pSrc[2 * i] - pSrc[2 * l];
        pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);
        yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
        pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);
        pSrc[2u * l] = xt;
        pSrc[2u * l + 1u] = yt;
        i += n1;
        l = i + n2;
        xt = pSrc[2 * i] - pSrc[2 * l];
        pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);
        yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
        pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);
        pSrc[2u * l] = xt;
        pSrc[2u * l + 1u] = yt;
    }
}

void arm_radix2_butterfly_inverse_q31(q31_t *pSrc, uint32_t fftLen, q31_t *pCoef, uint16_t twidCoefModifier)
{
    unsigned i, j, k, l;
    unsigned n1, n2, ia;
    q31_t xt, yt, cosVal, sinVal;
    q31_t p0, p1;

    n2 = fftLen;
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    for (i = 0; i < n2; i++) {
        cosVal = pCoef[ia * 2];
        sinVal = pCoef[(ia * 2) + 1];
        ia = ia + twidCoefModifier;
        l = i + n2;
        xt = (pSrc[2 * i] >> 1u) - (pSrc[2 * l] >> 1u);
        pSrc[2 * i] = ((pSrc[2 * i] >> 1u) + (pSrc[2 * l] >> 1u)) >> 1u;
        yt = (pSrc[2 * i + 1] >> 1u) - (pSrc[2 * l + 1] >> 1u);
        pSrc[2 * i + 1] = ((pSrc[2 * l + 1] >> 1u) + (pSrc[2 * i + 1] >> 1u)) >> 1u;
        p0 = (q31_t)(((q63_t)xt * cosVal) >> 32);
        p1 = (q31_t)(((q63_t)yt * cosVal) >> 32);
        p0 -= (q31_t)(((q63_t)yt * sinVal) >> 32);
        p1 += (q31_t)(((q63_t)xt * sinVal) >> 32);
        pSrc[2u * l] = p0;
        pSrc[2u * l + 1u] = p1;
    }

    twidCoefModifier = twidCoefModifier << 1u;

    for (k = fftLen / 2; k > 2; k = k >> 1) {
        n1 = n2;
        n2 = n2 >> 1;
        ia = 0;
        for (j = 0; j < n2; j++) {
            cosVal = pCoef[ia * 2];
            sinVal = pCoef[(ia * 2) + 1];
            ia = ia + twidCoefModifier;
            for (i = j; i < fftLen; i += n1) {
                l = i + n2;
                xt = pSrc[2 * i] - pSrc[2 * l];
                pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]) >> 1u;
                yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
                pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]) >> 1u;
                p0 = (q31_t)(((q63_t)xt * cosVal) >> 32);
                p1 = (q31_t)(((q63_t)yt * cosVal) >> 32);
                p0 -= (q31_t)(((q63_t)yt * sinVal) >> 32);
                p1 += (q31_t)(((q63_t)xt * sinVal) >> 32);
                pSrc[2u * l] = p0;
                pSrc[2u * l + 1u] = p1;
            }
        }
        twidCoefModifier = twidCoefModifier << 1u;
    }

    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;
    cosVal = pCoef[ia * 2];
    sinVal = pCoef[(ia * 2) + 1];
    ia = ia + twidCoefModifier;

    for (i = 0; i < fftLen; i += n1) {
        l = i + n2;
        xt = pSrc[2 * i] - pSrc[2 * l];
        pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);
        yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
        pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);
        pSrc[2u * l] = xt;
        pSrc[2u * l + 1u] = yt;
        i += n1;
        l = i + n2;
        xt = pSrc[2 * i] - pSrc[2 * l];
        pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);
        yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
        pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);
        pSrc[2u * l] = xt;
        pSrc[2u * l + 1u] = yt;
    }
}