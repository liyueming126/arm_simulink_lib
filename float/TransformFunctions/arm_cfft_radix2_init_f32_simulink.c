/* ----------------------------------------------------------------------
 * arm_cfft_radix2_init_f32_simulink.c
 * Simplified self-contained implementation of arm_cfft_radix2_init_f32.
 * Initializes the Radix-2 CFFT/CIFFT instance structure.
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Instance structure for Radix-2 CFFT/CIFFT */
typedef struct {
    uint16_t fftLen;
    uint8_t ifftFlag;
    uint8_t bitReverseFlag;
    float *pTwiddle;
    uint16_t *pBitRevTable;
    uint16_t twidCoefModifier;
    uint16_t bitRevFactor;
    float onebyfftLen;
} arm_cfft_radix2_instance_f32;

/* Generate twiddle factors for Radix-2 FFT: exp(-j*2*pi*k/N) for k=0..N/2-1 */
static float *generate_twiddle_radix2(uint16_t n)
{
    uint32_t i;
    uint32_t len = n / 2u;
    float *table = (float *)malloc(len * 2u * sizeof(float));
    if (!table) return 0;
    for (i = 0; i < len; i++) {
        double angle = -2.0 * 3.14159265358979323846 * (double)i / (double)n;
        table[2u * i]     = (float)cos(angle);
        table[2u * i + 1u] = (float)sin(angle);
    }
    return table;
}

/* Generate bit reversal table for Radix-2 FFT of length n */
static uint16_t *generate_bitrev_radix2(uint16_t n)
{
    uint32_t i;
    uint16_t *table;
    uint32_t bits = 0;
    uint32_t temp = n;
    /* Count bits needed */
    while (temp > 1) { temp >>= 1; bits++; }
    table = (uint16_t *)malloc((n - 4) * sizeof(uint16_t));
    if (!table) return 0;
    for (i = 0; i < n - 4; i++) {
        uint32_t rev = 0, idx = i + 1, b;
        for (b = 0; b < bits; b++) {
            rev = (rev << 1) | (idx & 1);
            idx >>= 1;
        }
        table[i] = (uint16_t)(rev >> 1);
    }
    return table;
}

int arm_cfft_radix2_init_f32(
  arm_cfft_radix2_instance_f32 * S,
  uint16_t fftLen,
  uint8_t ifftFlag,
  uint8_t bitReverseFlag)
{
  S->fftLen = fftLen;
  S->ifftFlag = ifftFlag;
  S->bitReverseFlag = bitReverseFlag;

  /* Generate twiddle table */
  S->pTwiddle = generate_twiddle_radix2(fftLen);

  /* Generate bit reversal table */
  S->pBitRevTable = generate_bitrev_radix2(fftLen);

  /* Set parameters for the given length */
  switch (fftLen) {
    case 4096: S->twidCoefModifier = 1u;    S->bitRevFactor = 1u;    S->onebyfftLen = 0.000244140625f;    break;
    case 2048: S->twidCoefModifier = 2u;    S->bitRevFactor = 2u;    S->onebyfftLen = 0.00048828125f;     break;
    case 1024: S->twidCoefModifier = 4u;    S->bitRevFactor = 4u;    S->onebyfftLen = 0.0009765625f;     break;
    case 512:  S->twidCoefModifier = 8u;    S->bitRevFactor = 8u;    S->onebyfftLen = 0.001953125f;      break;
    case 256:  S->twidCoefModifier = 16u;   S->bitRevFactor = 16u;   S->onebyfftLen = 0.00390625f;       break;
    case 128:  S->twidCoefModifier = 32u;   S->bitRevFactor = 32u;   S->onebyfftLen = 0.0078125f;        break;
    case 64:   S->twidCoefModifier = 64u;   S->bitRevFactor = 64u;   S->onebyfftLen = 0.015625f;         break;
    case 32:   S->twidCoefModifier = 128u;  S->bitRevFactor = 128u;  S->onebyfftLen = 0.03125f;          break;
    case 16:   S->twidCoefModifier = 256u;  S->bitRevFactor = 256u;  S->onebyfftLen = 0.0625f;           break;
    default:   return 1; /* ARM_MATH_ARGUMENT_ERROR */
  }
  return 0; /* ARM_MATH_SUCCESS */
}