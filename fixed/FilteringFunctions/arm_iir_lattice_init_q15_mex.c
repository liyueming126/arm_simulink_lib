#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef struct { uint16_t numStages; q15_t *pState; q15_t *pkCoeffs; q15_t *pvCoeffs; } arm_iir_lattice_instance_q15;

void arm_iir_lattice_init_q15(
    arm_iir_lattice_instance_q15 *S, uint16_t numStages,
    q15_t *pkCoeffs, q15_t *pvCoeffs, q15_t *pState, uint32_t blockSize)
{ S->numStages = numStages; S->pkCoeffs = pkCoeffs; S->pvCoeffs = pvCoeffs; memset(pState, 0, (numStages + blockSize) * sizeof(q15_t)); S->pState = pState; }

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_iir_lattice_instance_q15 S; uint16_t numStages; uint32_t blockSize; int32_t i; double *pPk, *pPv;
    if (nrhs < 4) mexErrMsgTxt("Need: numStages, pkCoeffs, pvCoeffs, blockSize");
    numStages = (uint16_t)(mxGetScalar(prhs[0]));
    pPk = mxGetPr(prhs[1]); q15_t *pkCoeffs = (q15_t *)mxMalloc(numStages * sizeof(q15_t));
    for (i = 0; i < numStages; i++) { double v = pPk[i] * 32768.0; if (v > 32767.0) v = 32767.0; if (v < -32768.0) v = -32768.0; pkCoeffs[i] = (q15_t)((int16_t)v); }
    pPv = mxGetPr(prhs[2]); q15_t *pvCoeffs = (q15_t *)mxMalloc((numStages + 1) * sizeof(q15_t));
    for (i = 0; i < numStages + 1; i++) { double v = pPv[i] * 32768.0; if (v > 32767.0) v = 32767.0; if (v < -32768.0) v = -32768.0; pvCoeffs[i] = (q15_t)((int16_t)v); }
    blockSize = (uint32_t)(mxGetScalar(prhs[3]));
    q15_t *pState = (q15_t *)mxMalloc((numStages + blockSize) * sizeof(q15_t));
    arm_iir_lattice_init_q15(&S, numStages, pkCoeffs, pvCoeffs, pState, blockSize);
    plhs[0] = mxCreateDoubleScalar((double)S.numStages);
    mxFree(pkCoeffs); mxFree(pvCoeffs); mxFree(pState);
}