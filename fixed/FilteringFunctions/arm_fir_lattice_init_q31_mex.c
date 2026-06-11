#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef struct { uint16_t numStages; q31_t *pState; q31_t *pCoeffs; } arm_fir_lattice_instance_q31;

void arm_fir_lattice_init_q31(arm_fir_lattice_instance_q31 *S, uint16_t numStages, q31_t *pCoeffs, q31_t *pState)
{ S->numStages = numStages; S->pCoeffs = pCoeffs; memset(pState, 0, numStages * sizeof(q31_t)); S->pState = pState; }

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_fir_lattice_instance_q31 S; uint16_t numStages; int32_t i; double *pCoefIn;
    if (nrhs < 2) mexErrMsgTxt("Need: numStages, coeffs");
    numStages = (uint16_t)(mxGetScalar(prhs[0]));
    pCoefIn = mxGetPr(prhs[1]); q31_t *pCoeffs = (q31_t *)mxMalloc(numStages * sizeof(q31_t));
    for (i = 0; i < numStages; i++) { double v = pCoefIn[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; pCoeffs[i] = (q31_t)((int32_t)v); }
    q31_t *pState = (q31_t *)mxMalloc(numStages * sizeof(q31_t));
    arm_fir_lattice_init_q31(&S, numStages, pCoeffs, pState);
    plhs[0] = mxCreateDoubleScalar((double)S.numStages); mxFree(pCoeffs); mxFree(pState);
}