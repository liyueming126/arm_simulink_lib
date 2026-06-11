
#include <stdint.h>
#include <math.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

static int clz_q31(q31_t x)
{
    int n = 0;
    uint32_t u = (uint32_t)x;
    while ((u & 0x80000000u) == 0u && n < 32) { u = u << 1u; n++; }
    return n;
}

static void arm_sqrt_q31(q31_t in, q31_t *pOut)
{
    q31_t number, temp1, bits_val1, var1, signBits1, half;
    float temp_float1;
    union { uint32_t fracval; float floatval; } tempconv;

    number = in;

    if (number > 0)
    {
        signBits1 = clz_q31(number) - 1;

        if ((signBits1 % 2) == 0)
            number = number << signBits1;
        else
            number = number << (signBits1 - 1);

        half = number >> 1;
        temp1 = number;

        temp_float1 = (float)number * 4.6566128731e-010f;
        tempconv.floatval = temp_float1;
        bits_val1 = (q31_t)tempconv.fracval;
        bits_val1 = 0x5f3759df - (bits_val1 >> 1);
        tempconv.fracval = (uint32_t)bits_val1;
        temp_float1 = tempconv.floatval;
        var1 = (q31_t)((int64_t)(temp_float1 * 1073741824.0));

        var1 = (q31_t)(((q63_t)var1 * (0x30000000 - ((q31_t)((((q31_t)(((q63_t)var1 * (q63_t)var1) >> 31)) * (q63_t)half) >> 31)))) >> 31) << 2;
        var1 = (q31_t)(((q63_t)var1 * (0x30000000 - ((q31_t)((((q31_t)(((q63_t)var1 * (q63_t)var1) >> 31)) * (q63_t)half) >> 31)))) >> 31) << 2;
        var1 = (q31_t)(((q63_t)var1 * (0x30000000 - ((q31_t)((((q31_t)(((q63_t)var1 * (q63_t)var1) >> 31)) * (q63_t)half) >> 31)))) >> 31) << 2;

        var1 = (q31_t)((((q63_t)temp1 * (q63_t)var1) >> 31) << 1);

        if ((signBits1 % 2) == 0)
            var1 = var1 >> (signBits1 / 2);
        else
            var1 = var1 >> ((signBits1 - 1) / 2);

        *pOut = var1;
    }
    else
    {
        *pOut = 0;
    }
}

void arm_cmplx_mag_q31(q31_t *pSrc, q31_t *pDst, uint32_t numSamples)
{
    q31_t real, imag;
    q31_t acc0, acc1;

    while (numSamples > 0u)
    {
        real = *pSrc++;
        imag = *pSrc++;
        acc0 = (q31_t)(((q63_t)real * (q63_t)real) >> 33);
        acc1 = (q31_t)(((q63_t)imag * (q63_t)imag) >> 33);
        arm_sqrt_q31(acc0 + acc1, pDst++);
        numSamples--;
    }
}
