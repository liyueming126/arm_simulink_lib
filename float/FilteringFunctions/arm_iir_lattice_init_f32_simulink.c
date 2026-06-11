/* Simplified implementation of ARM IIR lattice init for Simulink.
 * Based on CMSIS arm_iir_lattice_init_f32.c.
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

typedef struct {
    uint16_t numStages;
    float32_t *pState;
    float32_t *pkCoeffs;
    float32_t *pvCoeffs;
} arm_iir_lattice_instance_f32;

void arm_iir_lattice_init_f32(
    arm_iir_lattice_instance_f32 * S,
    uint16_t numStages,
    float32_t * pkCoeffs,
    float32_t * pvCoeffs,
    float32_t * pState,
    uint32_t blockSize)
{
    S->numStages = numStages;
    S->pkCoeffs = pkCoeffs;
    S->pvCoeffs = pvCoeffs;
    memset(pState, 0, (numStages + blockSize) * sizeof(float32_t));
    S->pState = pState;
}