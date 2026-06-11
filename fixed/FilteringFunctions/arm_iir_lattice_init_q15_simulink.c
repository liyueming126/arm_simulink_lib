
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef struct { uint16_t numStages; q15_t *pState; q15_t *pkCoeffs; q15_t *pvCoeffs; } arm_iir_lattice_instance_q15;

void arm_iir_lattice_init_q15(
    arm_iir_lattice_instance_q15 *S, uint16_t numStages,
    q15_t *pkCoeffs, q15_t *pvCoeffs, q15_t *pState, uint32_t blockSize)
{ S->numStages = numStages; S->pkCoeffs = pkCoeffs; S->pvCoeffs = pvCoeffs; memset(pState, 0, (numStages + blockSize) * sizeof(q15_t)); S->pState = pState; }
