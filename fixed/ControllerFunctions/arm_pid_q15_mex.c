#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int64_t q63_t;

typedef struct
{
    q15_t A0;
    q15_t A1;
    q15_t A2;
    q15_t state[3];
    q15_t Kp;
    q15_t Ki;
    q15_t Kd;
} arm_pid_instance_q15;

static inline int16_t __SSAT(int32_t val, int32_t sat)
{
    int32_t max = (1 << (sat - 1)) - 1;
    int32_t min = -(1 << (sat - 1));
    if (val > max) return (int16_t)max;
    if (val < min) return (int16_t)min;
    return (int16_t)val;
}

q15_t arm_pid_q15(
    arm_pid_instance_q15 * S,
    q15_t in)
{
    q63_t acc;
    q15_t out;

    acc = (q63_t)((int32_t)S->A0 * (int32_t)in);
    acc += (int32_t)S->A1 * (int32_t)S->state[0];
    acc += (int32_t)S->A2 * (int32_t)S->state[1];
    acc += (int32_t)S->state[2] << 15;

    out = __SSAT((int32_t)(acc >> 15), 16);

    S->state[1] = S->state[0];
    S->state[0] = in;
    S->state[2] = out;

    return out;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_pid_instance_q15 S;
    int16_t *pStruct, *pIn, *pOut;
    size_t blockSize, i;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_pid_q15:invalidInput", "2 inputs required: structVector[9], inSignal.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_pid_q15:invalidOutput", "At most 1 output allowed.");

    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxGetNumberOfElements(prhs[0]) != 9)
        mexErrMsgIdAndTxt("arm_pid_q15:invalidInputType", "Input 1 must be int16 vector of 9 elements.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]))
        mexErrMsgIdAndTxt("arm_pid_q15:invalidInputType", "Input 2 must be real int16 vector.");

    pStruct = (int16_t *)mxGetData(prhs[0]);
    pIn = (int16_t *)mxGetData(prhs[1]);
    blockSize = mxGetNumberOfElements(prhs[1]);

    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[1]),
        mxGetDimensions(prhs[1]), mxINT16_CLASS, mxREAL);
    pOut = (int16_t *)mxGetData(plhs[0]);

    S.A0 = pStruct[0];
    S.A1 = pStruct[1];
    S.A2 = pStruct[2];
    S.state[0] = pStruct[3];
    S.state[1] = pStruct[4];
    S.state[2] = pStruct[5];
    S.Kp = pStruct[6];
    S.Ki = pStruct[7];
    S.Kd = pStruct[8];

    for (i = 0; i < blockSize; i++)
    {
        pOut[i] = arm_pid_q15(&S, pIn[i]);
    }
}