/* ----------------------------------------------------------------------
 * arm_rfft_init_f32_simulink.c
 * Simplified self-contained implementation of arm_rfft_init_f32.
 * Initializes the RFFT/RIFFT instance structure with generated tables.
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Instance structure for RFFT/RIFFT (deprecated) */
typedef struct {
    uint32_t fftLenReal;
    uint32_t fftLenBy2;
    uint8_t ifftFlagR;
    uint8_t bitReverseFlagR;
    uint32_t twidCoefRModifier;
    float *pTwiddleAReal;
    float *pTwiddleBReal;
    void *pCfft;
} arm_rfft_instance_f32;

/* Radix-4 CFFT instance (used internally) */
typedef struct {
    uint16_t fftLen;
    uint8_t ifftFlag;
    uint8_t bitReverseFlag;
    float *pTwiddle;
    uint16_t *pBitRevTable;
    uint16_t twidCoefModifier;
    uint16_t bitRevFactor;
    float onebyfftLen;
} arm_cfft_radix4_instance_f32;

/* Generate realCoefA table:
   pATable[2*i]   = 0.5 * (1.0 - sin(2*PI*i/(2*n)))
   pATable[2*i+1] = 0.5 * (-1.0 * cos(2*PI*i/(2*n))) */
static float *generate_realCoefA(uint32_t n)
{
    uint32_t i;
    float *table = (float *)malloc(2u * n * sizeof(float));
    if (!table) return 0;
    for (i = 0; i < n; i++) {
        double angle = 2.0 * 3.14159265358979323846 * (double)i / (double)(2u * n);
        table[2u * i]     = (float)(0.5 * (1.0 - sin(angle)));
        table[2u * i + 1u] = (float)(0.5 * (-1.0 * cos(angle)));
    }
    return table;
}

/* Generate realCoefB table:
   pBTable[2*i]   = 0.5 * (1.0 + sin(2*PI*i/(2*n)))
   pBTable[2*i+1] = 0.5 * (1.0 * cos(2*PI*i/(2*n))) */
static float *generate_realCoefB(uint32_t n)
{
    uint32_t i;
    float *table = (float *)malloc(2u * n * sizeof(float));
    if (!table) return 0;
    for (i = 0; i < n; i++) {
        double angle = 2.0 * 3.14159265358979323846 * (double)i / (double)(2u * n);
        table[2u * i]     = (float)(0.5 * (1.0 + sin(angle)));
        table[2u * i + 1u] = (float)(0.5 * (1.0 * cos(angle)));
    }
    return table;
}

/* Generate twiddle factors for radix-4 CFFT */
static float *gen_twiddle_rfft(uint16_t n)
{
    uint32_t i;
    float *table = (float *)malloc(n * 2u * sizeof(float));
    if (!table) return 0;
    for (i = 0; i < n; i++) {
        double angle = -2.0 * 3.14159265358979323846 * (double)i / (double)n;
        table[2u * i]     = (float)cos(angle);
        table[2u * i + 1u] = (float)sin(angle);
    }
    return table;
}

/* Generate bit reversal table */
static uint16_t *gen_bitrev_rfft(uint16_t n)
{
    uint32_t i, bits = 0, temp = n;
    uint16_t *table;
    while (temp > 1) { temp >>= 1; bits++; }
    table = (uint16_t *)malloc((n - 4) * sizeof(uint16_t));
    if (!table) return 0;
    for (i = 0; i < n - 4; i++) {
        uint32_t rev = 0, idx = i + 1, b;
        for (b = 0; b < bits; b++) { rev = (rev << 1) | (idx & 1); idx >>= 1; }
        table[i] = (uint16_t)(rev >> 1);
    }
    return table;
}

int arm_rfft_init_f32(
  arm_rfft_instance_f32 * S,
  uint16_t fftLen,
  uint8_t ifftFlag,
  uint8_t bitReverseFlag)
{
    uint32_t n = fftLen;

    S->fftLenReal = fftLen;
    S->fftLenBy2 = fftLen / 2;
    S->ifftFlagR = ifftFlag;
    S->bitReverseFlagR = bitReverseFlag;

    S->pTwiddleAReal = generate_realCoefA(n);
    S->pTwiddleBReal = generate_realCoefB(n);

    /* Set modifier based on FFT length */
    switch (fftLen) {
        case 4096: S->twidCoefRModifier = 1u; break;
        case 2048: S->twidCoefRModifier = 2u; break;
        case 1024: S->twidCoefRModifier = 4u; break;
        case 512:  S->twidCoefRModifier = 8u; break;
        case 256:  S->twidCoefRModifier = 16u; break;
        case 128:  S->twidCoefRModifier = 32u; break;
        case 64:   S->twidCoefRModifier = 64u; break;
        case 32:   S->twidCoefRModifier = 128u; break;
        default:
            free(S->pTwiddleAReal); free(S->pTwiddleBReal);
            S->pTwiddleAReal = 0; S->pTwiddleBReal = 0;
            return 1;
    }

    /* pCfft is handled externally; for this init we just store NULL */
    S->pCfft = 0;

    return 0;
}

/* Free tables allocated by arm_rfft_init_f32 */
void arm_rfft_init_f32_free(arm_rfft_instance_f32 * S)
{
    if (S->pTwiddleAReal) free(S->pTwiddleAReal);
    if (S->pTwiddleBReal) free(S->pTwiddleBReal);
    S->pTwiddleAReal = 0;
    S->pTwiddleBReal = 0;
}