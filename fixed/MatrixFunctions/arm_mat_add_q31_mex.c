#include <stdint.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int64_t q63_t;
typedef int32_t arm_status;

#define ARM_MATH_SUCCESS       0
#define ARM_MATH_SIZE_MISMATCH 1

typedef struct {
	uint16_t numRows;
	uint16_t numCols;
	q31_t *pData;
} arm_matrix_instance_q31;

arm_status arm_mat_add_q31(
	const arm_matrix_instance_q31 *pSrcA,
	const arm_matrix_instance_q31 *pSrcB,
	arm_matrix_instance_q31 *pDst)
{
	q31_t *pIn1 = pSrcA->pData;
	q31_t *pIn2 = pSrcB->pData;
	q31_t *pOut = pDst->pData;
	uint32_t numSamples;
	uint32_t blkCnt;
	q63_t sum;

	if ((pSrcA->numRows != pSrcB->numRows) ||
		(pSrcA->numCols != pSrcB->numCols) ||
		(pSrcA->numRows != pDst->numRows) ||
		(pSrcA->numCols != pDst->numCols))
	{
		return ARM_MATH_SIZE_MISMATCH;
	}

	numSamples = (uint32_t)pSrcA->numRows * pSrcA->numCols;
	blkCnt = numSamples;

	while (blkCnt > 0u)
	{
		sum = (q63_t)*pIn1++ + (q63_t)*pIn2++;

		if (sum > (q63_t)INT32_MAX)
			sum = INT32_MAX;
		else if (sum < (q63_t)INT32_MIN)
			sum = INT32_MIN;

		*pOut++ = (q31_t)sum;
		blkCnt--;
	}

	return ARM_MATH_SUCCESS;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	int32_t *pSrcDataA, *pSrcDataB, *pDstData;
	size_t rowsA, colsA, rowsB, colsB;
	arm_matrix_instance_q31 srcA, srcB, dst;

	if (nrhs != 2)
		mexErrMsgIdAndTxt("arm_mat_add_q31:invalidInput", "2 inputs required (matrix A, matrix B).");
	if (nlhs > 1)
		mexErrMsgIdAndTxt("arm_mat_add_q31:invalidOutput", "At most 1 output allowed.");
	if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
		mexErrMsgIdAndTxt("arm_mat_add_q31:invalidInputType", "Input 1 must be real int32.");
	if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
		mexErrMsgIdAndTxt("arm_mat_add_q31:invalidInputType", "Input 2 must be real int32.");

	rowsA = mxGetM(prhs[0]);
	colsA = mxGetN(prhs[0]);
	rowsB = mxGetM(prhs[1]);
	colsB = mxGetN(prhs[1]);

	if (rowsA != rowsB || colsA != colsB)
		mexErrMsgIdAndTxt("arm_mat_add_q31:invalidInput", "Input matrices must have the same dimensions.");

	plhs[0] = mxCreateNumericMatrix(rowsA, colsA, mxINT32_CLASS, mxREAL);
	pSrcDataA = (int32_t *)mxGetData(prhs[0]);
	pSrcDataB = (int32_t *)mxGetData(prhs[1]);
	pDstData = (int32_t *)mxGetData(plhs[0]);

	srcA.numRows = (uint16_t)rowsA;
	srcA.numCols = (uint16_t)colsA;
	srcA.pData = pSrcDataA;

	srcB.numRows = (uint16_t)rowsB;
	srcB.numCols = (uint16_t)colsB;
	srcB.pData = pSrcDataB;

	dst.numRows = (uint16_t)rowsA;
	dst.numCols = (uint16_t)colsA;
	dst.pData = pDstData;

	arm_status status = arm_mat_add_q31(&srcA, &srcB, &dst);
	if (status == ARM_MATH_SIZE_MISMATCH)
		mexErrMsgIdAndTxt("arm_mat_add_q31:sizeMismatch", "Matrix dimension mismatch.");
}