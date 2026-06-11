/* ----------------------------------------------------------------------
 * arm_rfft_fast_init_f32_simulink.c
 * Simplified self-contained implementation of arm_rfft_fast_init_f32.
 * Initializes the fast RFFT instance structure.
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Fast RFFT instance structure */
typedef struct {
    uint16_t fftLenRFFT;
    float *pTwiddleRFFT;
    struct {
        uint16_t fftLen;
        const float *pTwiddle;
        const uint16_t *pBitRevTable;
        uint16_t bitRevLength;
    } Sint;
} arm_rfft_fast_instance_f32;

/* Generate twiddle factors for CFFT (internal): exp(-j*2*pi*k/N) for k=0..3*N/4-1 */
static float *gen_twiddle_cfft_rf(uint16_t n)
{
    uint32_t i, len = (3u * n) / 4u;
    float *table = (float *)malloc(len * 2u * sizeof(float));
    if (!table) return 0;
    for (i = 0; i < len; i++) {
        double angle = -2.0 * 3.14159265358979323846 * (double)i / (double)n;
        table[2u * i]     = (float)cos(angle);
        table[2u * i + 1u] = (float)sin(angle);
    }
    return table;
}

/* Generate RFFT twiddle factors: exp(-j*pi*k/N) for k=0..N-1, N = fftLenRFFT/2 */
static float *gen_twiddle_rfft_fast(uint16_t n)
{
    uint32_t i;
    float *table = (float *)malloc(2u * n * sizeof(float));
    if (!table) return 0;
    for (i = 0; i < n; i++) {
        double angle = -3.14159265358979323846 * (double)i / (double)n;
        table[2u * i]     = (float)cos(angle);
        table[2u * i + 1u] = (float)sin(angle);
    }
    return table;
}

/* Generate bit reversal table (byte-offset format for arm_bitreversal_32) */
static uint16_t *gen_bitrev_rfft_fast(uint16_t n)
{
    uint32_t i, bits = 0, temp = n;
    uint16_t *table;
    while (temp > 1) { temp >>= 1; bits++; }
    uint32_t tblLen = 2u * n;
    table = (uint16_t *)malloc(tblLen * sizeof(uint16_t));
    if (!table) return 0;
    uint32_t tIdx = 0;
    for (i = 0; i < n; i++) {
        uint32_t rev = 0, idx = i, b;
        for (b = 0; b < bits; b++) { rev = (rev << 1) | (idx & 1); idx >>= 1; }
        if (rev > i) {
            table[tIdx++] = (uint16_t)(2u * i * sizeof(float));
            table[tIdx++] = (uint16_t)(2u * rev * sizeof(float));
        }
    }
    return table;
}

int arm_rfft_fast_init_f32(
  arm_rfft_fast_instance_f32 * S,
  uint16_t fftLen)
{
  S->fftLenRFFT = fftLen;
  S->Sint.fftLen = fftLen / 2;

  /* Generate CFFT twiddle table */
  S->Sint.pTwiddle = gen_twiddle_cfft_rf(S->Sint.fftLen);

  /* Generate RFFT twiddle table */
  S->pTwiddleRFFT = gen_twiddle_rfft_fast(fftLen / 2);

  /* Generate bit reversal table */
  S->Sint.pBitRevTable = gen_bitrev_rfft_fast(S->Sint.fftLen);
  S->Sint.bitRevLength = 2u * S->Sint.fftLen;

  return 0;
}

/* Free the allocated tables */
void arm_rfft_fast_init_f32_free(arm_rfft_fast_instance_f32 * S)
{
    if (S->pTwiddleRFFT) free(S->pTwiddleRFFT);
    if (S->Sint.pTwiddle) free((void *)S->Sint.pTwiddle);
    if (S->Sint.pBitRevTable) free((void *)S->Sint.pBitRevTable);
    S->pTwiddleRFFT = 0;
    S->Sint.pTwiddle = 0;
    S->Sint.pBitRevTable = 0;
}