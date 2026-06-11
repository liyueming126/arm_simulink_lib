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

static inline int16_t __SSAT(int32_t val, int32_t sat)
{
    int32_t max = (1 << (sat - 1)) - 1;
    int32_t min = -(1 << (sat - 1));
    if (val > max) return (int16_t)max;
    if (val < min) return (int16_t)min;
    return (int16_t)val;
}

void arm_pid_init_q15(
    arm_pid_instance_q15 * S,
    int32_t resetStateFlag)
{
    int32_t temp;

    temp = (int32_t)S->Kp + (int32_t)S->Ki + (int32_t)S->Kd;
    S->A0 = __SSAT(temp, 16);

    temp = -((int32_t)S->Kd + (int32_t)S->Kd + (int32_t)S->Kp);
    S->A1 = __SSAT(temp, 16);
    S->A2 = S->Kd;

    if (resetStateFlag)
    {
        memset(S->state, 0, 3u * sizeof(q15_t));
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_pid_instance_q15 S;
    int16_t *pOut;

    if (nrhs != 4)
        mexErrMsgIdAndTxt("arm_pid_init_q15:invalidInput", "4 inputs required: Kp, Ki, Kd, resetStateFlag.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_pid_init_q15:invalidOutput", "At most 1 output allowed.");

    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxGetNumberOfElements(prhs[0]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_q15:invalidInputType", "Kp must be scalar int16.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_q15:invalidInputType", "Ki must be scalar int16.");
    if (!mxIsInt16(prhs[2]) || mxIsComplex(prhs[2]) || mxGetNumberOfElements(prhs[2]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_q15:invalidInputType", "Kd must be scalar int16.");
    if (!mxIsDouble(prhs[3]) || mxIsComplex(prhs[3]) || mxGetNumberOfElements(prhs[3]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_q15:invalidInputType", "resetStateFlag must be scalar double.");

    S.Kp = *(int16_t *)mxGetData(prhs[0]);
    S.Ki = *(int16_t *)mxGetData(prhs[1]);
    S.Kd = *(int16_t *)mxGetData(prhs[2]);
    int32_t resetFlag = (int32_t)mxGetScalar(prhs[3]);

    arm_pid_init_q15(&S, resetFlag);

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