
#include <stdint.h>
#include <math.h>

typedef int16_t q15_t;
typedef int64_t q63_t;

static int clz_q15(q15_t x)
{
    int n = 0;
    uint16_t u = (uint16_t)x;
    while ((u & 0x8000u) == 0u && n < 16) { u = u << 1u; n++; }
    return n + 16;
}

static void arm_sqrt_q15(q15_t in, q15_t *pOut)
{
    q15_t number, temp1, var1, signBits1, half;
    uint32_t bits_val1;
    float temp_float1;
    union { uint32_t fracval; float floatval; } tempconv;

    number = in;

    if (number > 0)
    {
        signBits1 = (q15_t)(clz_q15(number) - 17);

        if ((signBits1 % 2) == 0)
            number = number << signBits1;
        else
            number = number << (signBits1 - 1);

        half = number >> 1;
        temp1 = number;

        temp_float1 = (float)number * 3.051757812500000e-005f;
        tempconv.floatval = temp_float1;
        bits_val1 = tempconv.fracval;
        bits_val1 = 0x5f3759dfu - (bits_val1 >> 1u);
        tempconv.fracval = bits_val1;
        temp_float1 = tempconv.floatval;
        var1 = (q15_t)((int32_t)(temp_float1 * 16384.0f));

        var1 = (q15_t)(((int32_t)var1 * (0x3000 - ((q15_t)((((q15_t)(((int32_t)var1 * (int32_t)var1) >> 15)) * (int32_t)half) >> 15)))) >> 15) << 2;
        var1 = (q15_t)(((int32_t)var1 * (0x3000 - ((q15_t)((((q15_t)(((int32_t)var1 * (int32_t)var1) >> 15)) * (int32_t)half) >> 15)))) >> 15) << 2;
        var1 = (q15_t)(((int32_t)var1 * (0x3000 - ((q15_t)((((q15_t)(((int32_t)var1 * (int32_t)var1) >> 15)) * (int32_t)half) >> 15)))) >> 15) << 2;

        var1 = (q15_t)((((int32_t)temp1 * (int32_t)var1) >> 15) << 1);

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

void arm_cmplx_mag_q15(q15_t *pSrc, q15_t *pDst, uint32_t numSamples)
{
    q15_t real, imag;
    int32_t acc0, acc1;

    while (numSamples > 0u)
    {
        real = *pSrc++;
        imag = *pSrc++;
        acc0 = (int32_t)real * (int32_t)real;
        acc1 = (int32_t)imag * (int32_t)imag;
        arm_sqrt_q15((q15_t)(((q63_t)acc0 + (q63_t)acc1) >> 17), pDst++);
        numSamples--;
    }
}
