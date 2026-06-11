#include <stdint.h>

typedef int16_t q15_t;

typedef struct {
    uint16_t fftLen;
    uint8_t  ifftFlag;
    uint8_t  bitReverseFlag;
    uint16_t twidCoefModifier;
    uint16_t bitRevFactor;
} arm_cfft_radix2_instance_q15;

void arm_cfft_radix2_init_q15(
    arm_cfft_radix2_instance_q15 *S,
    uint16_t fftLen,
    uint8_t ifftFlag,
    uint8_t bitReverseFlag)
{
    S->fftLen = fftLen;
    S->ifftFlag = ifftFlag;
    S->bitReverseFlag = bitReverseFlag;

    switch (fftLen) {
    case 4096u: S->twidCoefModifier = 1u;   S->bitRevFactor = 1u;   break;
    case 2048u: S->twidCoefModifier = 2u;   S->bitRevFactor = 2u;   break;
    case 1024u: S->twidCoefModifier = 4u;   S->bitRevFactor = 4u;   break;
    case  512u: S->twidCoefModifier = 8u;   S->bitRevFactor = 8u;   break;
    case  256u: S->twidCoefModifier = 16u;  S->bitRevFactor = 16u;  break;
    case  128u: S->twidCoefModifier = 32u;  S->bitRevFactor = 32u;  break;
    case   64u: S->twidCoefModifier = 64u;  S->bitRevFactor = 64u;  break;
    case   32u: S->twidCoefModifier = 128u; S->bitRevFactor = 128u; break;
    case   16u: S->twidCoefModifier = 256u; S->bitRevFactor = 256u; break;
    default:    S->twidCoefModifier = 0;    S->bitRevFactor = 0;    break;
    }
}