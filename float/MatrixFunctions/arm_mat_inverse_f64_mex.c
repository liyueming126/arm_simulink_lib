#include "arm_mat_inverse_f64_simulink.c"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_matrix_instance_f64 matSrc, matDst;
    double *pSrc, *pD;
    size_t numRows, numCols;
    arm_status status;

    if(nrhs != 1) mexErrMsgTxt("One input required.");
    if(nlhs > 1) mexErrMsgTxt("One output required.");
    if(!mxIsDouble(prhs[0]) || mxIsComplex(prhs[0])) mexErrMsgTxt("Input must be real double.");

    numRows = mxGetM(prhs[0]);
    numCols = mxGetN(prhs[0]);

    if(numRows != numCols) mexErrMsgTxt("Matrix must be square.");

    pSrc = (double *)mxGetData(prhs[0]);
    matSrc.numRows = (uint16_t)numRows; matSrc.numCols = (uint16_t)numCols; matSrc.pData = pSrc;

    plhs[0] = mxCreateNumericMatrix(numRows, numCols, mxDOUBLE_CLASS, mxREAL);
    pD = (double *)mxGetData(plhs[0]);
    matDst.numRows = (uint16_t)numRows; matDst.numCols = (uint16_t)numCols; matDst.pData = pD;

    status = arm_mat_inverse_f64(&matSrc, &matDst);
    if(status == ARM_MATH_SIZE_MISMATCH) mexErrMsgTxt("Size mismatch.");
    if(status == ARM_MATH_SINGULAR) mexErrMsgTxt("Matrix is singular.");
}