
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef struct { uint16_t numStages; q31_t *pState; q31_t *pCoeffs; } arm_fir_lattice_instance_q31;

void arm_fir_lattice_init_q31(arm_fir_lattice_instance_q31 *S, uint16_t numStages, q31_t *pCoeffs, q31_t *pState)
{ S->numStages = numStages; S->pCoeffs = pCoeffs; memset(pState, 0, numStages * sizeof(q31_t)); S->pState = pState; }
