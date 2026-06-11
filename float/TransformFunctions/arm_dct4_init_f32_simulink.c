/* ----------------------------------------------------------------------
 * arm_dct4_init_f32_simulink.c
 * Simplified self-contained implementation of arm_dct4_init_f32.
 * Initializes the DCT4 instance structure with generated tables.
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* DCT4 instance structure */
typedef struct {
    uint16_t N;
    uint16_t Nby2;
    float normalize;
    float *pTwiddle;
    float *pCosFactor;
    void *pRfft;
} arm_dct4_instance_f32;

/* RFFT instance (deprecated) - used internally */
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

/* CFFT instance */
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

/* Generate Weights table: weights[n] = e^(-j*n*pi/(2*N))
   weights[2*i] = cos(i*pi/(2*N)), weights[2*i+1] = -sin(i*pi/(2*N)) */
static float *generate_weights_dct4(uint16_t N)
{
    uint32_t i;
    float *table = (float *)malloc(2u * N * sizeof(float));
    if (!table) return 0;
    for (i = 0; i < N; i++) {
        double angle = 3.14159265358979323846 * (double)i / (double)(2u * N);
        table[2u * i]     = (float)cos(angle);
        table[2u * i + 1u] = (float)(-sin(angle));
    }
    return table;
}

/* Generate Cos Factor table: cosFact[n] = cos(pi*(2*n+1)/(4*N)) */
static float *generate_cosfact_dct4(uint16_t N)
{
    uint32_t i;
    float *table = (float *)malloc(N * sizeof(float));
    if (!table) return 0;
    for (i = 0; i < N; i++) {
        double angle = 3.14159265358979323846 * (double)(2u * i + 1u) / (double)(4u * N);
        table[i] = (float)cos(angle);
    }
    return table;
}

int arm_dct4_init_f32(
  arm_dct4_instance_f32 * S,
  uint16_t N,
  uint16_t Nby2,
  float normalize)
{
    S->N = N;
    S->Nby2 = Nby2;
    S->normalize = normalize;

    /* Generate weights table */
    S->pTwiddle = generate_weights_dct4(N);
    if (!S->pTwiddle) return 1;

    /* Generate cos factor table */
    S->pCosFactor = generate_cosfact_dct4(N);
    if (!S->pCosFactor) {
        free(S->pTwiddle);
        S->pTwiddle = 0;
        return 1;
    }

    /* pRfft is handled externally */
    S->pRfft = 0;

    return 0;
}

void arm_dct4_init_f32_free(arm_dct4_instance_f32 * S)
{
    if (S->pTwiddle) free(S->pTwiddle);
    if (S->pCosFactor) free(S->pCosFactor);
    S->pTwiddle = 0;
    S->pCosFactor = 0;
}