
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef struct { uint16_t numStages; q31_t *pState; q31_t *pkCoeffs; q31_t *pvCoeffs; } arm_iir_lattice_instance_q31;

void arm_iir_lattice_init_q31(
    arm_iir_lattice_instance_q31 *S, uint16_t numStages,
    q31_t *pkCoeffs, q31_t *pvCoeffs, q31_t *pState, uint32_t blockSize)
{ S->numStages = numStages; S->pkCoeffs = pkCoeffs; S->pvCoeffs = pvCoeffs; memset(pState, 0, (numStages + blockSize) * sizeof(q31_t)); S->pState = pState; }
