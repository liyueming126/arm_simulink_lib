/* Simplified implementation of ARM Biquad stereo DF2T filter for Simulink.
 * Based on CMSIS arm_biquad_cascade_stereo_df2T_f32.c (Cortex-M0 version).
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

typedef struct {
    uint8_t numStages;
    float32_t *pState;
    float32_t *pCoeffs;
} arm_biquad_cascade_stereo_df2T_instance_f32;

void arm_biquad_cascade_stereo_df2T_f32(
    const arm_biquad_cascade_stereo_df2T_instance_f32 * S,
    float32_t * pSrc,
    float32_t * pDst,
    uint32_t blockSize)
{
    float32_t *pIn = pSrc;
    float32_t *pOut = pDst;
    float32_t *pState = S->pState;
    float32_t *pCoeffs = S->pCoeffs;
    float32_t acc1a, acc1b;
    float32_t b0, b1, b2, a1, a2;
    float32_t Xn1a, Xn1b;
    float32_t d1a, d2a, d1b, d2b;
    uint32_t sample, stage = S->numStages;

    do
    {
        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;

        d1a = pState[0];
        d2a = pState[1];
        d1b = pState[2];
        d2b = pState[3];

        sample = blockSize;

        while(sample > 0u)
        {
            Xn1a = *pIn++;
            Xn1b = *pIn++;

            acc1a = (b0 * Xn1a) + d1a;
            acc1b = (b0 * Xn1b) + d1b;

            *pOut++ = acc1a;
            *pOut++ = acc1b;

            d1a = ((b1 * Xn1a) + (a1 * acc1a)) + d2a;
            d1b = ((b1 * Xn1b) + (a1 * acc1b)) + d2b;

            d2a = (b2 * Xn1a) + (a2 * acc1a);
            d2b = (b2 * Xn1b) + (a2 * acc1b);

            sample--;
        }

        *pState++ = d1a;
        *pState++ = d2a;
        *pState++ = d1b;
        *pState++ = d2b;

        pIn = pDst;
        pOut = pDst;
        stage--;
    } while(stage > 0u);
}