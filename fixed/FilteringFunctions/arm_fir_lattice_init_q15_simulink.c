
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef struct { uint16_t numStages; q15_t *pState; q15_t *pCoeffs; } arm_fir_lattice_instance_q15;

void arm_fir_lattice_init_q15(arm_fir_lattice_instance_q15 *S, uint16_t numStages, q15_t *pCoeffs, q15_t *pState)
{ S->numStages = numStages; S->pCoeffs = pCoeffs; memset(pState, 0, numStages * sizeof(q15_t)); S->pState = pState; }
