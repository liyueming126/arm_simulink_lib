#include <stdint.h>

typedef int16_t q15_t;
typedef int32_t q31_t;

typedef struct {
    uint16_t fftLen;
    uint8_t  ifftFlag;
    uint8_t  bitReverseFlag;
    uint16_t twidCoefModifier;
    uint16_t bitRevFactor;
    q15_t   *pTwiddle;
    uint16_t *pBitRevTable;
} arm_cfft_radix2_instance_q15;

static int16_t ssat16(int32_t val) {
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (int16_t)val;
}

void arm_bitreversal_q15(q15_t *pSrc16, uint32_t fftLen, uint16_t bitRevFactor, uint16_t *pBitRevTab)
{
    q31_t *pSrc = (q31_t *)pSrc16;
    q31_t in;
    uint32_t fftLenBy2, fftLenBy2p1, i, j;

    j = 0u;
    fftLenBy2 = fftLen / 2u;
    fftLenBy2p1 = (fftLen / 2u) + 1u;
    for (i = 0u; i <= (fftLenBy2 - 2u); i += 2u)
    {
        if (i < j) {
            in = pSrc[i]; pSrc[i] = pSrc[j]; pSrc[j] = in;
            in = pSrc[i + fftLenBy2p1]; pSrc[i + fftLenBy2p1] = pSrc[j + fftLenBy2p1]; pSrc[j + fftLenBy2p1] = in;
        }
        in = pSrc[i + 1u]; pSrc[i + 1u] = pSrc[j + fftLenBy2]; pSrc[j + fftLenBy2] = in;
        j = *pBitRevTab;
        pBitRevTab += bitRevFactor;
    }
}

void arm_radix2_butterfly_q15(q15_t *pSrc, uint32_t fftLen, q15_t *pCoef, uint16_t twidCoefModifier);
void arm_radix2_butterfly_inverse_q15(q15_t *pSrc, uint32_t fftLen, q15_t *pCoef, uint16_t twidCoefModifier);

void arm_cfft_radix2_q15(const arm_cfft_radix2_instance_q15 *S, q15_t *pSrc)
{
    if (S->ifftFlag == 1u)
        arm_radix2_butterfly_inverse_q15(pSrc, S->fftLen, S->pTwiddle, S->twidCoefModifier);
    else
        arm_radix2_butterfly_q15(pSrc, S->fftLen, S->pTwiddle, S->twidCoefModifier);
    arm_bitreversal_q15(pSrc, S->fftLen, S->bitRevFactor, S->pBitRevTable);
}

void arm_radix2_butterfly_q15(q15_t *pSrc, uint32_t fftLen, q15_t *pCoef, uint16_t twidCoefModifier)
{
    unsigned i, j, k, l;
    unsigned n1, n2, ia;
    q15_t xt, yt, cosVal, sinVal;

    n2 = fftLen;
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    for (j = 0; j < n2; j++) {
        cosVal = pCoef[ia * 2];
        sinVal = pCoef[(ia * 2) + 1];
        ia = ia + twidCoefModifier;
        for (i = j; i < fftLen; i += n1) {
            l = i + n2;
            xt = (pSrc[2 * i] >> 1u) - (pSrc[2 * l] >> 1u);
            pSrc[2 * i] = ((pSrc[2 * i] >> 1u) + (pSrc[2 * l] >> 1u)) >> 1u;
            yt = (pSrc[2 * i + 1] >> 1u) - (pSrc[2 * l + 1] >> 1u);
            pSrc[2 * i + 1] = ((pSrc[2 * l + 1] >> 1u) + (pSrc[2 * i + 1] >> 1u)) >> 1u;
            pSrc[2u * l] = (q15_t)((((q31_t)xt * cosVal) >> 16) + (((q31_t)yt * sinVal) >> 16));
            pSrc[2u * l + 1u] = (q15_t)((((q31_t)yt * cosVal) >> 16) - (((q31_t)xt * sinVal) >> 16));
        }
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
                pSrc[2u * l] = (q15_t)((((q31_t)xt * cosVal) >> 16) + (((q31_t)yt * sinVal) >> 16));
                pSrc[2u * l + 1u] = (q15_t)((((q31_t)yt * cosVal) >> 16) - (((q31_t)xt * sinVal) >> 16));
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
    }
}

void arm_radix2_butterfly_inverse_q15(q15_t *pSrc, uint32_t fftLen, q15_t *pCoef, uint16_t twidCoefModifier)
{
    unsigned i, j, k, l;
    unsigned n1, n2, ia;
    q15_t xt, yt, cosVal, sinVal;

    n2 = fftLen;
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    for (j = 0; j < n2; j++) {
        cosVal = pCoef[ia * 2];
        sinVal = pCoef[(ia * 2) + 1];
        ia = ia + twidCoefModifier;
        for (i = j; i < fftLen; i += n1) {
            l = i + n2;
            xt = (pSrc[2 * i] >> 1u) - (pSrc[2 * l] >> 1u);
            pSrc[2 * i] = ((pSrc[2 * i] >> 1u) + (pSrc[2 * l] >> 1u)) >> 1u;
            yt = (pSrc[2 * i + 1] >> 1u) - (pSrc[2 * l + 1] >> 1u);
            pSrc[2 * i + 1] = ((pSrc[2 * l + 1] >> 1u) + (pSrc[2 * i + 1] >> 1u)) >> 1u;
            pSrc[2u * l] = (q15_t)((((q31_t)xt * cosVal) >> 16) - (((q31_t)yt * sinVal) >> 16));
            pSrc[2u * l + 1u] = (q15_t)((((q31_t)yt * cosVal) >> 16) + (((q31_t)xt * sinVal) >> 16));
        }
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
                pSrc[2u * l] = (q15_t)((((q31_t)xt * cosVal) >> 16) - (((q31_t)yt * sinVal) >> 16));
                pSrc[2u * l + 1u] = (q15_t)((((q31_t)yt * cosVal) >> 16) + (((q31_t)xt * sinVal) >> 16));
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
    }
}