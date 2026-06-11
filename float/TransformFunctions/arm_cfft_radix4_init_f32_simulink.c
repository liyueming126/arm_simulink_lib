/* ----------------------------------------------------------------------
 * arm_cfft_radix4_init_f32_simulink.c
 * Simplified self-contained implementation of arm_cfft_radix4_init_f32.
 * Initializes the Radix-4 CFFT/CIFFT instance structure.
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Instance structure for Radix-4 CFFT/CIFFT */
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

/* Generate twiddle factors for Radix-4 FFT: exp(-j*2*pi*k/N) for k=0..N-1 */
static float *generate_twiddle_radix4(uint16_t n)
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
static uint16_t *generate_bitrev_radix4(uint16_t n)
{
    uint32_t i;
    uint16_t *table;
    uint32_t bits = 0;
    uint32_t temp = n;
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

int arm_cfft_radix4_init_f32(
  arm_cfft_radix4_instance_f32 * S,
  uint16_t fftLen,
  uint8_t ifftFlag,
  uint8_t bitReverseFlag)
{
  S->fftLen = fftLen;
  S->ifftFlag = ifftFlag;
  S->bitReverseFlag = bitReverseFlag;

  /* Generate twiddle table */
  S->pTwiddle = generate_twiddle_radix4(fftLen);

  /* Generate bit reversal table */
  S->pBitRevTable = generate_bitrev_radix4(fftLen);

  /* Set parameters for the given length */
  switch (fftLen) {
    case 4096: S->twidCoefModifier = 1u;   S->bitRevFactor = 1u;   S->onebyfftLen = 0.000244140625f;   break;
    case 1024: S->twidCoefModifier = 4u;   S->bitRevFactor = 4u;   S->onebyfftLen = 0.0009765625f;    break;
    case 256:  S->twidCoefModifier = 16u;  S->bitRevFactor = 16u;  S->onebyfftLen = 0.00390625f;      break;
    case 64:   S->twidCoefModifier = 64u;  S->bitRevFactor = 64u;  S->onebyfftLen = 0.015625f;        break;
    case 16:   S->twidCoefModifier = 256u; S->bitRevFactor = 256u; S->onebyfftLen = 0.0625f;          break;
    default:   return 1;
  }
  return 0;
}