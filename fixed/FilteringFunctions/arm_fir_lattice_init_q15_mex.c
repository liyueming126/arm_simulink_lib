#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef struct { uint16_t numStages; q15_t *pState; q15_t *pCoeffs; } arm_fir_lattice_instance_q15;

void arm_fir_lattice_init_q15(arm_fir_lattice_instance_q15 *S, uint16_t numStages, q15_t *pCoeffs, q15_t *pState)
{ S->numStages = numStages; S->pCoeffs = pCoeffs; memset(pState, 0, numStages * sizeof(q15_t)); S->pState = pState; }

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_fir_lattice_instance_q15 S; uint16_t numStages; int32_t i; double *pCoefIn;
    if (nrhs < 2) mexErrMsgTxt("Need: numStages, coeffs");
    numStages = (uint16_t)(mxGetScalar(prhs[0]));
    pCoefIn = mxGetPr(prhs[1]); q15_t *pCoeffs = (q15_t *)mxMalloc(numStages * sizeof(q15_t));
    for (i = 0; i < numStages; i++) { double v = pCoefIn[i] * 32768.0; if (v > 32767.0) v = 32767.0; if (v < -32768.0) v = -32768.0; pCoeffs[i] = (q15_t)((int16_t)v); }
    q15_t *pState = (q15_t *)mxMalloc(numStages * sizeof(q15_t));
    arm_fir_lattice_init_q15(&S, numStages, pCoeffs, pState);
    plhs[0] = mxCreateDoubleScalar((double)S.numStages);
    mxFree(pCoeffs); mxFree(pState);
}