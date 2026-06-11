#include <stdint.h>

typedef int16_t q15_t;

typedef struct {
    uint16_t fftLen;
    uint8_t  ifftFlag;
    uint8_t  bitReverseFlag;
    uint16_t twidCoefModifier;
    uint16_t bitRevFactor;
} arm_cfft_radix4_instance_q15;

void arm_cfft_radix4_init_q15(
    arm_cfft_radix4_instance_q15 *S,
    uint16_t fftLen,
    uint8_t ifftFlag,
    uint8_t bitReverseFlag)
{
    S->fftLen = fftLen;
    S->ifftFlag = ifftFlag;
    S->bitReverseFlag = bitReverseFlag;

    switch (fftLen) {
    case 4096u: S->twidCoefModifier = 1u;  S->bitRevFactor = 1u;  break;
    case 1024u: S->twidCoefModifier = 4u;  S->bitRevFactor = 4u;  break;
    case  256u: S->twidCoefModifier = 16u; S->bitRevFactor = 16u; break;
    case   64u: S->twidCoefModifier = 64u; S->bitRevFactor = 64u; break;
    case   16u: S->twidCoefModifier = 256u;S->bitRevFactor = 256u;break;
    default:    S->twidCoefModifier = 0;   S->bitRevFactor = 0;   break;
    }
}