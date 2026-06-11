/* Simplified implementation of ARM FIR lattice init for Simulink.
 * Based on CMSIS arm_fir_lattice_init_f32.c.
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

typedef struct {
    uint16_t numStages;
    float32_t *pState;
    float32_t *pCoeffs;
} arm_fir_lattice_instance_f32;

void arm_fir_lattice_init_f32(
    arm_fir_lattice_instance_f32 * S,
    uint16_t numStages,
    float32_t * pCoeffs,
    float32_t * pState)
{
    S->numStages = numStages;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (numStages) * sizeof(float32_t));
    S->pState = pState;
}