#include <stdint.h>

typedef int16_t q15_t;

typedef struct {
    uint16_t N;
    uint16_t Nby2;
    int16_t  normalize;
} arm_dct4_instance_q15;

void arm_dct4_init_q15(
    arm_dct4_instance_q15 *S,
    uint16_t N,
    int16_t normalize)
{
    S->N = N;
    S->Nby2 = N >> 1;
    S->normalize = normalize;
}