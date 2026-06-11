
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint32_t numStages;
    q31_t *pState;
    q31_t *pCoeffs;
    uint8_t postShift;
} arm_biquad_casd_df1_inst_q31;

void arm_biquad_cascade_df1_fast_q31(
    const arm_biquad_casd_df1_inst_q31 *S,
    q31_t *pSrc,
    q31_t *pDst,
    uint32_t blockSize)
{
    q31_t acc = 0;
    q31_t Xn1, Xn2, Yn1, Yn2;
    q31_t b0, b1, b2, a1, a2;
    q31_t *pIn = pSrc;
    q31_t *pOut = pDst;
    q31_t *pState = S->pState;
    q31_t *pCoeffs = S->pCoeffs;
    q31_t Xn;
    int32_t shift = (int32_t)S->postShift + 1;
    uint32_t sample, stage = S->numStages;

    do {
        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;
        Xn1 = pState[0]; Xn2 = pState[1]; Yn1 = pState[2]; Yn2 = pState[3];

        sample = blockSize >> 2u;

        while (sample > 0u) {
            Xn = *pIn;
            acc = (q31_t)(((q63_t)b0 * Xn) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)b1 * Xn1)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)b2 * Xn2)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)a1 * Yn1)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)a2 * Yn2)) >> 32);
            Yn2 = acc << shift;
            Xn2 = *(pIn + 1u); *pOut = Yn2;

            acc = (q31_t)(((q63_t)b0 * Xn2) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)b1 * Xn)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)b2 * Xn1)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)a1 * Yn2)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)a2 * Yn1)) >> 32);
            Yn1 = acc << shift;
            Xn1 = *(pIn + 2u); *(pOut + 1u) = Yn1;

            acc = (q31_t)(((q63_t)b0 * Xn1) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)b1 * Xn2)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)b2 * Xn)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)a1 * Yn1)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)a2 * Yn2)) >> 32);
            Yn2 = acc << shift;
            Xn = *(pIn + 3u); *(pOut + 2u) = Yn2;
            pIn += 4u;

            acc = (q31_t)(((q63_t)b0 * Xn) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)b1 * Xn1)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)b2 * Xn2)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)a1 * Yn2)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)a2 * Yn1)) >> 32);
            Xn2 = Xn1; Yn1 = acc << shift; Xn1 = Xn;
            *(pOut + 3u) = Yn1; pOut += 4u; sample--;
        }

        sample = (blockSize & 0x3u);
        while (sample > 0u) {
            Xn = *pIn++;
            acc = (q31_t)(((q63_t)b0 * Xn) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)b1 * Xn1)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)b2 * Xn2)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)a1 * Yn1)) >> 32);
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)a2 * Yn2)) >> 32);
            acc = acc << shift;
            Xn2 = Xn1; Xn1 = Xn; Yn2 = Yn1; Yn1 = acc;
            *pOut++ = acc; sample--;
        }

        pIn = pDst; pOut = pDst;
        *pState++ = Xn1; *pState++ = Xn2; *pState++ = Yn1; *pState++ = Yn2;
    } while (--stage);
}
