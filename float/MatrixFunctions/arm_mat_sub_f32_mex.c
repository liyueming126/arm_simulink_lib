#include "arm_mat_sub_f32_simulink.c"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_matrix_instance_f32 matA, matB, matD;
    float *pA, *pB, *pD;
    size_t numRows, numCols;
    arm_status status;

    if(nrhs != 2) mexErrMsgTxt("Two inputs required.");
    if(nlhs > 1) mexErrMsgTxt("One output required.");
    if(!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0])) mexErrMsgTxt("Input A must be real single.");
    if(!mxIsSingle(prhs[1]) || mxIsComplex(prhs[1])) mexErrMsgTxt("Input B must be real single.");

    numRows = mxGetM(prhs[0]);
    numCols = mxGetN(prhs[0]);
    if(mxGetM(prhs[1]) != numRows || mxGetN(prhs[1]) != numCols)
        mexErrMsgTxt("Matrix dimensions must match.");

    pA = (float *)mxGetData(prhs[0]);
    pB = (float *)mxGetData(prhs[1]);

    matA.numRows = (uint16_t)numRows; matA.numCols = (uint16_t)numCols; matA.pData = pA;
    matB.numRows = (uint16_t)numRows; matB.numCols = (uint16_t)numCols; matB.pData = pB;

    plhs[0] = mxCreateNumericMatrix(numRows, numCols, mxSINGLE_CLASS, mxREAL);
    pD = (float *)mxGetData(plhs[0]);
    matD.numRows = (uint16_t)numRows; matD.numCols = (uint16_t)numCols; matD.pData = pD;

    status = arm_mat_sub_f32(&matA, &matB, &matD);
    if(status != ARM_MATH_SUCCESS) mexErrMsgTxt("Matrix subtraction failed.");
}