
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int32_t q31_t;

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

void arm_biquad_cascade_df1_fast_q15(
    const arm_biquad_casd_df1_inst_q15 *S,
    q15_t *pSrc,
    q15_t *pDst,
    uint32_t blockSize)
{
    q15_t *pIn = pSrc;
    q15_t *pOut = pDst;
    q31_t acc;
    int32_t shift = (int32_t)(15 - S->postShift);
    q15_t *pState = S->pState;
    q15_t *pCoeffs = S->pCoeffs;
    uint32_t sample, stage = S->numStages;

    do {
        q15_t b0, b1, b2, a1, a2;
        q15_t x_n1, x_n2, y_n1, y_n2;

        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;
        pCoeffs++; /* skip padding */

        x_n1 = pState[0];
        x_n2 = pState[1];
        y_n1 = pState[2];
        y_n2 = pState[3];
        pState += 4;

        sample = blockSize;

        while (sample > 0u) {
            q15_t in = *pIn++;

            /* acc = b0*in + b1*x_n1 + b2*x_n2 - a1*y_n1 - a2*y_n2 */
            acc  = (q31_t)b0 * in;
            acc += (q31_t)b1 * x_n1;
            acc += (q31_t)b2 * x_n2;
            acc -= (q31_t)a1 * y_n1;
            acc -= (q31_t)a2 * y_n2;

            q15_t out = ssat16(acc >> shift);
            *pOut++ = out;

            /* Update state shift registers */
            x_n2 = x_n1;
            x_n1 = in;
            y_n2 = y_n1;
            y_n1 = out;

            sample--;
        }

        pIn = pDst;
        pOut = pDst;

        pState -= 4;
        pState[0] = x_n1;
        pState[1] = x_n2;
        pState[2] = y_n1;
        pState[3] = y_n2;
        pState += 4;

        stage--;
    } while (stage > 0u);
}
