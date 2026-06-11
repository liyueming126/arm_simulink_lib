#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int16_t q15_t;

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

void arm_pid_reset_q15(
    arm_pid_instance_q15 * S)
{
    memset(S->state, 0, 3u * sizeof(q15_t));
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_pid_instance_q15 S;
    int16_t *pIn, *pOut;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_pid_reset_q15:invalidInput", "1 input required: struct vector [A0,A1,A2,state0,state1,state2,Kp,Ki,Kd].");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_pid_reset_q15:invalidOutput", "At most 1 output allowed.");

    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxGetNumberOfElements(prhs[0]) != 9)
        mexErrMsgIdAndTxt("arm_pid_reset_q15:invalidInputType", "Input must be int16 vector of 9 elements.");

    pIn = (int16_t *)mxGetData(prhs[0]);
    S.A0 = pIn[0];
    S.A1 = pIn[1];
    S.A2 = pIn[2];
    S.state[0] = pIn[3];
    S.state[1] = pIn[4];
    S.state[2] = pIn[5];
    S.Kp = pIn[6];
    S.Ki = pIn[7];
    S.Kd = pIn[8];

    arm_pid_reset_q15(&S);

    plhs[0] = mxCreateNumericMatrix(1, 9, mxINT16_CLASS, mxREAL);
    pOut = (int16_t *)mxGetData(plhs[0]);
    pOut[0] = S.A0;
    pOut[1] = S.A1;
    pOut[2] = S.A2;
    pOut[3] = S.state[0];
    pOut[4] = S.state[1];
    pOut[5] = S.state[2];
    pOut[6] = S.Kp;
    pOut[7] = S.Ki;
    pOut[8] = S.Kd;
}