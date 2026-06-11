
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint32_t numStages;
    q15_t *pState;
    q15_t *pCoeffs;
    int8_t postShift;
} arm_biquad_casd_df1_inst_q15;

static q15_t ssat16(int32_t val) {
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (q15_t)val;
}

void arm_biquad_cascade_df1_q15(
    const arm_biquad_casd_df1_inst_q15 *S,
    q15_t *pSrc,
    q15_t *pDst,
    uint32_t blockSize)
{
    q15_t *pIn = pSrc;
    q15_t *pOut = pDst;
    q15_t b0, b1, b2, a1, a2;
    q15_t Xn1, Xn2, Yn1, Yn2;
    q15_t Xn;
    q63_t acc;
    int32_t shift = (15 - (int32_t)S->postShift);
    q15_t *pState = S->pState;
    q15_t *pCoeffs = S->pCoeffs;
    uint32_t sample, stage = (uint32_t)S->numStages;

    do {
        b0 = *pCoeffs++;
        pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;

        Xn1 = pState[0];
        Xn2 = pState[1];
        Yn1 = pState[2];
        Yn2 = pState[3];

        sample = blockSize;

        while (sample > 0u) {
            Xn = *pIn++;
            acc = (q31_t)b0 * Xn;
            acc += (q31_t)b1 * Xn1;
            acc += (q31_t)b2 * Xn2;
            acc += (q31_t)a1 * Yn1;
            acc += (q31_t)a2 * Yn2;

            acc = ssat16((acc >> shift));

            Xn2 = Xn1;
            Xn1 = Xn;
            Yn2 = Yn1;
            Yn1 = (q15_t)acc;

            *pOut++ = (q15_t)acc;
            sample--;
        }

        pIn = pDst;
        pOut = pDst;

        *pState++ = Xn1;
        *pState++ = Xn2;
        *pState++ = Yn1;
        *pState++ = Yn2;

    } while (--stage);
}
