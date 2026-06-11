#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef struct { uint16_t numStages; q31_t *pState; q31_t *pkCoeffs; q31_t *pvCoeffs; } arm_iir_lattice_instance_q31;

void arm_iir_lattice_init_q31(
    arm_iir_lattice_instance_q31 *S, uint16_t numStages,
    q31_t *pkCoeffs, q31_t *pvCoeffs, q31_t *pState, uint32_t blockSize)
{ S->numStages = numStages; S->pkCoeffs = pkCoeffs; S->pvCoeffs = pvCoeffs; memset(pState, 0, (numStages + blockSize) * sizeof(q31_t)); S->pState = pState; }

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_iir_lattice_instance_q31 S; uint16_t numStages; uint32_t blockSize; int32_t i; double *pPk, *pPv;
    if (nrhs < 4) mexErrMsgTxt("Need: numStages, pkCoeffs, pvCoeffs, blockSize");
    numStages = (uint16_t)(mxGetScalar(prhs[0]));
    pPk = mxGetPr(prhs[1]); q31_t *pkCoeffs = (q31_t *)mxMalloc(numStages * sizeof(q31_t));
    for (i = 0; i < numStages; i++) { double v = pPk[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; pkCoeffs[i] = (q31_t)((int32_t)v); }
    pPv = mxGetPr(prhs[2]); q31_t *pvCoeffs = (q31_t *)mxMalloc((numStages + 1) * sizeof(q31_t));
    for (i = 0; i < numStages + 1; i++) { double v = pPv[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; pvCoeffs[i] = (q31_t)((int32_t)v); }
    blockSize = (uint32_t)(mxGetScalar(prhs[3]));
    q31_t *pState = (q31_t *)mxMalloc((numStages + blockSize) * sizeof(q31_t));
    arm_iir_lattice_init_q31(&S, numStages, pkCoeffs, pvCoeffs, pState, blockSize);
    plhs[0] = mxCreateDoubleScalar((double)S.numStages);
    mxFree(pkCoeffs); mxFree(pvCoeffs); mxFree(pState);
}