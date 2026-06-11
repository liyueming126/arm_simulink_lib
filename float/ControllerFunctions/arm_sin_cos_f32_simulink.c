#include <stdint.h>
#include <math.h>

void arm_sin_cos_f32(
    float theta,
    float * pSinVal,
    float * pCosVal)
{
    *pSinVal = sinf(theta * 3.14159265358979f / 180.0f);
    *pCosVal = cosf(theta * 3.14159265358979f / 180.0f);
}