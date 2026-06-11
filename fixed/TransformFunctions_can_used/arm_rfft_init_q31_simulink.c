#include <stdint.h>

typedef int32_t q31_t;

typedef struct {
    uint16_t fftLenReal;
    uint8_t  ifftFlagR;
    uint8_t  bitReverseFlagR;
    uint16_t twidCoefRModifier;
} arm_rfft_instance_q31;

void arm_rfft_init_q31(
    arm_rfft_instance_q31 *S,
    uint16_t fftLenReal,
    uint8_t ifftFlagR,
    uint8_t bitReverseFlagR)
{
    uint32_t fftLen = fftLenReal >> 1;

    S->fftLenReal = fftLenReal;
    S->ifftFlagR = ifftFlagR;
    S->bitReverseFlagR = bitReverseFlagR;

    switch (fftLen) {
    case 4096u: S->twidCoefRModifier = 1u;   break;
    case 2048u: S->twidCoefRModifier = 2u;   break;
    case 1024u: S->twidCoefRModifier = 4u;   break;
    case  512u: S->twidCoefRModifier = 8u;   break;
    case  256u: S->twidCoefRModifier = 16u;  break;
    case  128u: S->twidCoefRModifier = 32u;  break;
    case   64u: S->twidCoefRModifier = 64u;  break;
    case   32u: S->twidCoefRModifier = 128u; break;
    default:    S->twidCoefRModifier = 0;    break;
    }
}