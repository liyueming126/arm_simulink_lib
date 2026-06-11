#include "arm_mat_cmplx_mult_f32_simulink.c"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_matrix_instance_f32 matA, matB, matD;
    float *pA, *pB, *pD;
    size_t numRowsA, numColsA, numRowsB, numColsB;
    arm_status status;

    if(nrhs != 2) mexErrMsgTxt("Two inputs required.");
    if(nlhs > 1) mexErrMsgTxt("One output required.");
    if(!mxIsSingle(prhs[0])) mexErrMsgTxt("Input A must be single.");
    if(!mxIsSingle(prhs[1])) mexErrMsgTxt("Input B must be single.");
    if(!mxIsComplex(prhs[0])) mexErrMsgTxt("Input A must be complex.");
    if(!mxIsComplex(prhs[1])) mexErrMsgTxt("Input B must be complex.");

    numRowsA = mxGetM(prhs[0]);
    numColsA = mxGetN(prhs[0]);
    numRowsB = mxGetM(prhs[1]);
    numColsB = mxGetN(prhs[1]);

    if(numColsA != numRowsB)
        mexErrMsgTxt("Inner dimensions must match: A.cols == B.rows.");

    pA = (float *)mxGetData(prhs[0]);
    pB = (float *)mxGetData(prhs[1]);

    matA.numRows = (uint16_t)numRowsA; matA.numCols = (uint16_t)numColsA; matA.pData = pA;
    matB.numRows = (uint16_t)numRowsB; matB.numCols = (uint16_t)numColsB; matB.pData = pB;

    plhs[0] = mxCreateNumericMatrix(numRowsA, numColsB, mxSINGLE_CLASS, mxCOMPLEX);
    pD = (float *)mxGetData(plhs[0]);
    matD.numRows = (uint16_t)numRowsA; matD.numCols = (uint16_t)numColsB; matD.pData = pD;

    status = arm_mat_cmplx_mult_f32(&matA, &matB, &matD);
    if(status != ARM_MATH_SUCCESS) mexErrMsgTxt("Complex matrix multiplication failed.");
}