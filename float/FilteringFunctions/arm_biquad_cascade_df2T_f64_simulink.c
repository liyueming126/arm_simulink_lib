/* Simplified implementation of ARM Biquad DF2T filter for Simulink (f64).
 * Based on CMSIS arm_biquad_cascade_df2T_f64.c (Cortex-M0 version).
 */

#include <stdint.h>
#include <string.h>

typedef double float64_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

typedef struct {
    uint8_t numStages;
    float64_t *pState;
    float64_t *pCoeffs;
} arm_biquad_cascade_df2T_instance_f64;

void arm_biquad_cascade_df2T_f64(
    const arm_biquad_cascade_df2T_instance_f64 * S,
    float64_t * pSrc,
    float64_t * pDst,
    uint32_t blockSize)
{
    float64_t *pIn = pSrc;
    float64_t *pOut = pDst;
    float64_t *pState = S->pState;
    float64_t *pCoeffs = S->pCoeffs;
    float64_t acc1;
    float64_t b0, b1, b2, a1, a2;
    float64_t Xn1;
    float64_t d1, d2;
    uint32_t sample, stage = S->numStages;

    do
    {
        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;

        d1 = pState[0];
        d2 = pState[1];

        sample = blockSize;

        while(sample > 0u)
        {
            Xn1 = *pIn++;
            acc1 = (b0 * Xn1) + d1;
            *pOut++ = acc1;
            d1 = ((b1 * Xn1) + (a1 * acc1)) + d2;
            d2 = (b2 * Xn1) + (a2 * acc1);
            sample--;
        }

        *pState++ = d1;
        *pState++ = d2;

        pIn = pDst;
        pOut = pDst;
        stage--;
    } while(stage > 0u);
}