#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct
{
    q31_t A0;
    q31_t A1;
    q31_t A2;
    q31_t state[3];
    q31_t Kp;
    q31_t Ki;
    q31_t Kd;
} arm_pid_instance_q31;

static inline q31_t clip_q63_to_q31(q63_t x)
{
    return ((q31_t)(x >> 32) != ((q31_t)x >> 31))
        ? ((int32_t)0x7FFFFFFF ^ ((q31_t)(x >> 63)))
        : (q31_t)x;
}

void arm_pid_init_q31(
    arm_pid_instance_q31 * S,
    int32_t resetStateFlag)
{
    q31_t temp;

    temp = clip_q63_to_q31((q63_t)S->Kp + S->Ki);
    S->A0 = clip_q63_to_q31((q63_t)temp + S->Kd);

    temp = clip_q63_to_q31((q63_t)S->Kd + S->Kd);
    S->A1 = -clip_q63_to_q31((q63_t)temp + S->Kp);

    S->A2 = S->Kd;

    if (resetStateFlag)
    {
        memset(S->state, 0, 3u * sizeof(q31_t));
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_pid_instance_q31 S;
    int32_t *pOut;

    if (nrhs != 4)
        mexErrMsgIdAndTxt("arm_pid_init_q31:invalidInput", "4 inputs required: Kp, Ki, Kd, resetStateFlag.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_pid_init_q31:invalidOutput", "At most 1 output allowed.");

    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxGetNumberOfElements(prhs[0]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_q31:invalidInputType", "Kp must be scalar int32.");
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_q31:invalidInputType", "Ki must be scalar int32.");
    if (!mxIsInt32(prhs[2]) || mxIsComplex(prhs[2]) || mxGetNumberOfElements(prhs[2]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_q31:invalidInputType", "Kd must be scalar int32.");
    if (!mxIsDouble(prhs[3]) || mxIsComplex(prhs[3]) || mxGetNumberOfElements(prhs[3]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_q31:invalidInputType", "resetStateFlag must be scalar double.");

    S.Kp = *(int32_t *)mxGetData(prhs[0]);
    S.Ki = *(int32_t *)mxGetData(prhs[1]);
    S.Kd = *(int32_t *)mxGetData(prhs[2]);
    int32_t resetFlag = (int32_t)mxGetScalar(prhs[3]);

    arm_pid_init_q31(&S, resetFlag);

    plhs[0] = mxCreateNumericMatrix(1, 9, mxINT32_CLASS, mxREAL);
    pOut = (int32_t *)mxGetData(plhs[0]);
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