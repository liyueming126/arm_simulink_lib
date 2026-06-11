#include <stdint.h>

typedef int32_t q31_t;

typedef struct {
    uint16_t N;
    uint16_t Nby2;
    q31_t    normalize;
} arm_dct4_instance_q31;

void arm_dct4_init_q31(
    arm_dct4_instance_q31 *S,
    uint16_t N,
    q31_t normalize)
{
    S->N = N;
    S->Nby2 = N >> 1;
    S->normalize = normalize;
}