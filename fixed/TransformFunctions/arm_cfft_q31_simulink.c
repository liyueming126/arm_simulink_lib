/* ----------------------------------------------------------------------
 * arm_cfft_q31_simulink.c
 * Self-contained Q31 complex FFT for Simulink.
 * Uses non-CM7 code paths from CMSIS DSP Library (plain C).
 * All dependencies inlined.
 *
 * Based on CMSIS DSP Library arm_cfft_q31.c, arm_cfft_radix4_q31.c,
 * and arm_bitreversal.c (non-CM7 paths).
 * -------------------------------------------------------------------- */

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
 * Type definitions
 * -------------------------------------------------------------------- */
#ifndef Q31_T_DEFINED
typedef int32_t q31_t;
#define Q31_T_DEFINED
#endif
#ifndef Q63_T_DEFINED
typedef int64_t q63_t;
#define Q63_T_DEFINED
#endif

/* --------------------------------------------------------------------
 * CMSIS-compatible struct types
 * -------------------------------------------------------------------- */
#ifndef ARM_CFFT_INSTANCE_Q31_DEFINED
typedef struct {
    uint32_t fftLen;
    q31_t *pTwiddle;
    uint16_t *pBitRevTable;
    uint16_t bitRevLength;
} arm_cfft_instance_q31;
#define ARM_CFFT_INSTANCE_Q31_DEFINED
#endif

/* --------------------------------------------------------------------
 * Multiply-accumulate helpers (replaces CMSIS macros for CMx)
 * -------------------------------------------------------------------- */
#ifndef mult_32x32_keep32_R
#define mult_32x32_keep32_R(a, x, y) \
    a = (q31_t)(((q63_t)(x) * (y) + 0x80000000LL) >> 32)
#endif

#ifndef multAcc_32x32_keep32_R
#define multAcc_32x32_keep32_R(a, x, y) \
    a = (q31_t)(((((q63_t)(a) << 32) + (q63_t)(x) * (y) + 0x80000000LL)) >> 32)
#endif

#ifndef multSub_32x32_keep32_R
#define multSub_32x32_keep32_R(a, x, y) \
    a = (q31_t)(((((q63_t)(a) << 32) - (q63_t)(x) * (y) + 0x80000000LL)) >> 32)
#endif

/* --------------------------------------------------------------------
 * Saturation helper
 * -------------------------------------------------------------------- */
static inline q31_t ssat_q31(q63_t x)
{
    if (x > 2147483647LL) return 2147483647;
    if (x < -2147483648LL) return -2147483648;
    return (q31_t)x;
}

/* --------------------------------------------------------------------
 * Pre-computed twiddle factor tables (Q31)
 * Generated as: round(cos(i*2*PI/N) * 2147483647), round(sin(i*2*PI/N) * 2147483647)
 * -------------------------------------------------------------------- */

/* 16-point: 3*16/4 = 12 complex pairs = 24 entries */
static const q31_t twiddleCoef_16_q31[24] = {
    0x7FFFFFFF, 0x00000000,
    0x7641AF3C, 0x30FBC54D,
    0x5A82799A, 0x5A82799A,
    0x30FBC54D, 0x7641AF3C,
    0x00000000, 0x7FFFFFFF,
    0xCF043AB2, 0x7641AF3C,
    0xA57D8666, 0x5A82799A,
    0x89BE50C3, 0x30FBC54D,
    0x80000000, 0x00000000,
    0x89BE50C3, 0xCF043AB2,
    0xA57D8666, 0xA57D8666,
    0xCF043AB2, 0x89BE50C3
};

/* 32-point: 3*32/4 = 24 complex pairs = 48 entries */
static const q31_t twiddleCoef_32_q31[48] = {
    0x7FFFFFFF, 0x00000000,
    0x7D8A5F3F, 0x18F8B83C,
    0x7641AF3C, 0x30FBC54D,
    0x6A6D98A4, 0x471CECE6,
    0x5A82799A, 0x5A82799A,
    0x471CECE6, 0x6A6D98A4,
    0x30FBC54D, 0x7641AF3C,
    0x18F8B83C, 0x7D8A5F3F,
    0x00000000, 0x7FFFFFFF,
    0xE70747C3, 0x7D8A5F3F,
    0xCF043AB2, 0x7641AF3C,
    0xB8E31319, 0x6A6D98A4,
    0xA57D8666, 0x5A82799A,
    0x9592675B, 0x471CECE6,
    0x89BE50C3, 0x30FBC54D,
    0x8275A0C0, 0x18F8B83C,
    0x80000000, 0x00000000,
    0x8275A0C0, 0xE70747C3,
    0x89BE50C3, 0xCF043AB2,
    0x9592675B, 0xB8E31319,
    0xA57D8666, 0xA57D8666,
    0xB8E31319, 0x9592675B,
    0xCF043AB2, 0x89BE50C3,
    0xE70747C3, 0x8275A0C0
};

/* 64-point: 3*64/4 = 48 complex pairs = 96 entries */
static const q31_t twiddleCoef_64_q31[96] = {
    0x7FFFFFFF, 0x00000000,
    0x7F62368F, 0x0C8BD35E,
    0x7D8A5F3F, 0x18F8B83C,
    0x7A7D055B, 0x25280C5D,
    0x7641AF3C, 0x30FBC54D,
    0x70E2CBC6, 0x3C56BA70,
    0x6A6D98A4, 0x471CECE6,
    0x62F201AC, 0x5133CC94,
    0x5A82799A, 0x5A82799A,
    0x5133CC94, 0x62F201AC,
    0x471CECE6, 0x6A6D98A4,
    0x3C56BA70, 0x70E2CBC6,
    0x30FBC54D, 0x7641AF3C,
    0x25280C5D, 0x7A7D055B,
    0x18F8B83C, 0x7D8A5F3F,
    0x0C8BD35E, 0x7F62368F,
    0x00000000, 0x7FFFFFFF,
    0xF3742CA1, 0x7F62368F,
    0xE70747C3, 0x7D8A5F3F,
    0xDAD7F3A2, 0x7A7D055B,
    0xCF043AB2, 0x7641AF3C,
    0xC3A9458F, 0x70E2CBC6,
    0xB8E31319, 0x6A6D98A4,
    0xAECC336B, 0x62F201AC,
    0xA57D8666, 0x5A82799A,
    0x9D0DFE53, 0x5133CC94,
    0x9592675B, 0x471CECE6,
    0x8F1D343A, 0x3C56BA70,
    0x89BE50C3, 0x30FBC54D,
    0x8582FAA2, 0x25280C5D,
    0x8275A0C0, 0x18F8B83C,
    0x809DC970, 0x0C8BD35E,
    0x80000000, 0x00000000,
    0x809DC970, 0xF3742CA1,
    0x8275A0C0, 0xE70747C3,
    0x8582FAA2, 0xDAD7F3A2,
    0x89BE50C3, 0xCF043AB2,
    0x8F1D343A, 0xC3A9458F,
    0x9592675B, 0xB8E31319,
    0x9D0DFE53, 0xAECC336B,
    0xA57D8666, 0xA57D8666,
    0xAECC336B, 0x9D0DFE53,
    0xB8E31319, 0x9592675B,
    0xC3A9458F, 0x8F1D343A,
    0xCF043AB2, 0x89BE50C3,
    0xDAD7F3A2, 0x8582FAA2,
    0xE70747C3, 0x8275A0C0,
    0xF3742CA1, 0x809DC970
};

/* --------------------------------------------------------------------
 * Twiddle table generation function for Q31
 * Generate twiddle factors for any N that is a power of 2, 16 <= N <= 4096
 * -------------------------------------------------------------------- */
static q31_t *generate_twiddle_q31(uint32_t n)
{
    uint32_t i, len = (3u * n) / 4u;
    q31_t *table = (q31_t *)malloc(len * 2u * sizeof(q31_t));
    if (!table) return 0;
    for (i = 0; i < len; i++) {
        double angle = 2.0 * 3.14159265358979323846 * (double)i / (double)n;
        double c = cos(angle);
        double s = sin(angle);
        table[2u * i]     = (q31_t)((int64_t)(c * 2147483647.0 + (c >= 0 ? 0.5 : -0.5)));
        table[2u * i + 1u] = (q31_t)((int64_t)(s * 2147483647.0 + (s >= 0 ? 0.5 : -0.5)));
    }
    return table;
}

/* --------------------------------------------------------------------
 * Bit-reversal table data (fixed-point)
 * -------------------------------------------------------------------- */
#define ARMBITREVINDEXTABLE_FIXED___16_TABLE_LENGTH 12u
#define ARMBITREVINDEXTABLE_FIXED___32_TABLE_LENGTH 24u
#define ARMBITREVINDEXTABLE_FIXED___64_TABLE_LENGTH 56u
#define ARMBITREVINDEXTABLE_FIXED__128_TABLE_LENGTH 112u
#define ARMBITREVINDEXTABLE_FIXED__256_TABLE_LENGTH 240u
#define ARMBITREVINDEXTABLE_FIXED__512_TABLE_LENGTH 480u
#define ARMBITREVINDEXTABLE_FIXED_1024_TABLE_LENGTH 992u
#define ARMBITREVINDEXTABLE_FIXED_2048_TABLE_LENGTH 1984u
#define ARMBITREVINDEXTABLE_FIXED_4096_TABLE_LENGTH 4032u

static const uint16_t armBitRevIndexTable_fixed_16_q31[] = {
    8,64, 16,32, 24,96, 40,80, 56,112, 88,104
};

static const uint16_t armBitRevIndexTable_fixed_32_q31[] = {
    8,128, 16,64, 24,192, 40,160, 48,96, 56,224, 72,144,
    88,208, 104,176, 120,240, 152,200, 184,232
};

static const uint16_t armBitRevIndexTable_fixed_64_q31[] = {
    8,256, 16,128, 24,384, 32,64, 40,320, 48,192, 56,448,
    72,288, 80,160, 88,416, 96,224, 104,480, 112,352, 120,224,
    136,320, 144,192, 152,448, 168,384, 176,288, 184,480,
    200,416, 208,352, 216,352, 232,448, 240,416, 248,480,
    264,448, 272,416, 280,416, 296,480, 304,448, 312,480,
    328,480, 344,480, 360,480, 376,480, 392,480, 408,480,
    424,480, 440,480, 456,480, 472,480
};

static const uint16_t armBitRevIndexTable_fixed_128_q31[] = {
    8,512, 16,256, 24,768, 32,128, 40,640, 48,384, 56,896,
    64,64, 72,576, 80,320, 88,832, 96,192, 104,704, 112,448,
    120,960, 136,576, 144,320, 152,832, 160,192, 168,704,
    176,448, 184,960, 200,640, 208,384, 216,896, 224,256,
    232,768, 240,512, 248,1024, 264,704, 272,448, 280,960,
    288,320, 296,832, 304,576, 312,1088, 328,768, 336,512,
    344,1024, 352,384, 360,896, 368,640, 376,1152, 392,832,
    400,576, 408,1088, 416,448, 424,960, 432,704, 440,1216,
    456,896, 464,640, 472,1152, 480,512, 488,1024, 496,768,
    504,1280, 520,960, 528,704, 536,1216, 544,576, 552,1088,
    560,832, 568,1344, 584,1024, 592,768, 600,1280, 608,640,
    616,1152, 624,896, 632,1408, 648,1088, 656,832, 664,1344,
    672,704, 680,1216, 688,960, 696,1472, 712,1152, 720,896,
    728,1408, 736,768, 744,1280, 752,1024, 760,1536, 776,1216,
    784,960, 792,1472, 800,832, 808,1344, 816,1088, 824,1600,
    840,1280, 848,1024, 856,1536, 864,896, 872,1408, 880,1152,
    888,1664, 904,1344, 912,1088, 920,1600, 928,960, 936,1472,
    944,1216, 952,1728, 968,1408, 976,1152, 984,1664, 992,1024,
    1000,1536, 1008,1280, 1016,1792
};

static const uint16_t armBitRevIndexTable_fixed_256_q31[] = {
    8,1024, 16,512, 24,1536, 32,256, 40,1280, 48,768, 56,1792,
    64,128, 72,1152, 80,640, 88,1664, 96,384, 104,1408, 112,896,
    120,1920, 136,1152, 144,640, 152,1664, 160,384, 168,1408,
    176,896, 184,1920, 200,1280, 208,768, 216,1792, 224,512,
    232,1536, 240,1024, 248,2048, 256,1152, 264,1664, 272,1408,
    280,1920, 288,1152, 296,1664, 304,1408, 312,1920, 328,1280,
    336,1792, 344,2560, 352,1152, 360,1664, 368,1408, 376,1920,
    392,1664, 400,1408, 408,1920, 424,1792, 432,2560, 440,3328,
    456,1664, 464,1408, 472,1920, 488,1664, 496,1408, 504,1920,
    520,1792, 528,2560, 536,3328, 552,1664, 560,1408, 568,1920,
    584,1792, 592,2560, 600,3328, 616,1664, 624,1408, 632,1920,
    648,1792, 656,2560, 664,3328, 680,1664, 688,1408, 696,1920,
    712,1792, 720,2560, 728,3328, 744,1664, 752,1408, 760,1920,
    776,1792, 784,2560, 792,3328, 808,1664, 816,1408, 824,1920,
    840,1792, 848,2560, 856,3328, 872,1664, 880,1408, 888,1920,
    904,1792, 912,2560, 920,3328, 936,1664, 944,1408, 952,1920,
    968,1792, 976,2560, 984,3328, 1000,1792, 1008,2560, 1016,3328
};

/* --------------------------------------------------------------------
 * arm_bitreversal_32 - operates on uint32_t*, matches CMSIS assembly
 * behavior: table entries in pairs, used directly as byte offsets.
 * Swaps 8 bytes (2 uint32_t) between the two addresses.
 * -------------------------------------------------------------------- */
static void arm_bitreversal_32(uint32_t *pSrc, const uint16_t bitRevLen, const uint16_t *pBitRevTable)
{
    uint16_t i;
    for (i = 0; i < bitRevLen; i += 2) {
        uint32_t offset1 = pBitRevTable[i];
        uint32_t offset2 = pBitRevTable[i+1];
        uint32_t *p1 = (uint32_t *)((uint8_t *)pSrc + offset1);
        uint32_t *p2 = (uint32_t *)((uint8_t *)pSrc + offset2);
        uint32_t t0 = p1[0], t1 = p1[1];
        p1[0] = p2[0]; p1[1] = p2[1];
        p2[0] = t0; p2[1] = t1;
    }
}

/* ----------------------------------------------------------------
 * arm_radix4_butterfly_q31 - non-CM7 path
 * ---------------------------------------------------------------- */
static void arm_radix4_butterfly_q31(
    q31_t *pSrc,
    uint32_t fftLen,
    q31_t *pCoef,
    uint32_t twidCoefModifier)
{
    /* non-CM7 path */
    uint32_t n1, n2, ia1, ia2, ia3, i0, j, k;
    q31_t t1, t2, r1, r2, s1, s2, co1, co2, co3, si1, si2, si3;
    q31_t xa, xb, xc, xd;
    q31_t ya, yb, yc, yd;
    q31_t xa_out, xb_out, xc_out, xd_out;
    q31_t ya_out, yb_out, yc_out, yd_out;

    q31_t *ptr1;
    q31_t *pSi0;
    q31_t *pSi1;
    q31_t *pSi2;
    q31_t *pSi3;
    q63_t xaya, xbyb, xcyc, xdyd;

    /* Total process is divided into three stages */

    /* start of first stage process */
    n2 = fftLen;
    n1 = n2;
    n2 >>= 2u;

    ia1 = 0u;

    j = n2;

    pSi0 = pSrc;
    pSi1 = pSi0 + 2 * n2;
    pSi2 = pSi1 + 2 * n2;
    pSi3 = pSi2 + 2 * n2;

    /* Calculation of first stage */
    do {
        /* input is in 1.31(q31) format and provide 4 guard bits */

        /* xa + xc */
        r1 = (pSi0[0] >> 4u) + (pSi2[0] >> 4u);
        /* xa - xc */
        r2 = (pSi0[0] >> 4u) - (pSi2[0] >> 4u);

        /* xb + xd */
        t1 = (pSi1[0] >> 4u) + (pSi3[0] >> 4u);

        /* ya + yc */
        s1 = (pSi0[1] >> 4u) + (pSi2[1] >> 4u);
        /* ya - yc */
        s2 = (pSi0[1] >> 4u) - (pSi2[1] >> 4u);

        /* xa' = xa + xb + xc + xd */
        *pSi0++ = (r1 + t1);
        /* (xa + xc) - (xb + xd) */
        r1 = r1 - t1;
        /* yb + yd */
        t2 = (pSi1[1] >> 4u) + (pSi3[1] >> 4u);

        /* ya' = ya + yb + yc + yd */
        *pSi0++ = (s1 + t2);

        /* (ya + yc) - (yb + yd) */
        s1 = s1 - t2;

        /* yb - yd */
        t1 = (pSi1[1] >> 4u) - (pSi3[1] >> 4u);
        /* xb - xd */
        t2 = (pSi1[0] >> 4u) - (pSi3[0] >> 4u);

        /* index calculation for the coefficients */
        ia2 = 2u * ia1;
        co2 = pCoef[ia2 * 2u];
        si2 = pCoef[(ia2 * 2u) + 1u];

        /* xc' = (xa-xb+xc-xd)co2 + (ya-yb+yc-yd)(si2) */
        *pSi1++ = (((int32_t)(((q63_t)r1 * co2) >> 32)) +
                    ((int32_t)(((q63_t)s1 * si2) >> 32))) << 1u;

        /* yc' = (ya-yb+yc-yd)co2 - (xa-xb+xc-xd)(si2) */
        *pSi1++ = (((int32_t)(((q63_t)s1 * co2) >> 32)) -
                    ((int32_t)(((q63_t)r1 * si2) >> 32))) << 1u;

        /* (xa - xc) + (yb - yd) */
        r1 = r2 + t1;
        /* (xa - xc) - (yb - yd) */
        r2 = r2 - t1;

        /* (ya - yc) - (xb - xd) */
        s1 = s2 - t2;
        /* (ya - yc) + (xb - xd) */
        s2 = s2 + t2;

        co1 = pCoef[ia1 * 2u];
        si1 = pCoef[(ia1 * 2u) + 1u];

        /* xb' = (xa+yb-xc-yd)co1 + (ya-xb-yc+xd)(si1) */
        *pSi2++ = (((int32_t)(((q63_t)r1 * co1) >> 32)) +
                    ((int32_t)(((q63_t)s1 * si1) >> 32))) << 1u;

        /* yb' = (ya-xb-yc+xd)co1 - (xa+yb-xc-yd)(si1) */
        *pSi2++ = (((int32_t)(((q63_t)s1 * co1) >> 32)) -
                    ((int32_t)(((q63_t)r1 * si1) >> 32))) << 1u;

        /* index calculation for the coefficients */
        ia3 = 3u * ia1;
        co3 = pCoef[ia3 * 2u];
        si3 = pCoef[(ia3 * 2u) + 1u];

        /* xd' = (xa-yb-xc+yd)co3 + (ya+xb-yc-xd)(si3) */
        *pSi3++ = (((int32_t)(((q63_t)r2 * co3) >> 32)) +
                    ((int32_t)(((q63_t)s2 * si3) >> 32))) << 1u;

        /* yd' = (ya+xb-yc-xd)co3 - (xa-yb-xc+yd)(si3) */
        *pSi3++ = (((int32_t)(((q63_t)s2 * co3) >> 32)) -
                    ((int32_t)(((q63_t)r2 * si3) >> 32))) << 1u;

        /* Twiddle coefficients index modifier */
        ia1 = ia1 + twidCoefModifier;

    } while (--j);

    /* end of first stage process */
    /* data is in 5.27(q27) format */

    /* start of Middle stages process */
    twidCoefModifier <<= 2u;

    for (k = fftLen / 4u; k > 4u; k >>= 2u) {
        n1 = n2;
        n2 >>= 2u;
        ia1 = 0u;

        for (j = 0u; j <= (n2 - 1u); j++) {
            ia2 = ia1 + ia1;
            ia3 = ia2 + ia1;
            co1 = pCoef[ia1 * 2u];
            si1 = pCoef[(ia1 * 2u) + 1u];
            co2 = pCoef[ia2 * 2u];
            si2 = pCoef[(ia2 * 2u) + 1u];
            co3 = pCoef[ia3 * 2u];
            si3 = pCoef[(ia3 * 2u) + 1u];
            ia1 = ia1 + twidCoefModifier;

            pSi0 = pSrc + 2 * j;
            pSi1 = pSi0 + 2 * n2;
            pSi2 = pSi1 + 2 * n2;
            pSi3 = pSi2 + 2 * n2;

            for (i0 = j; i0 < fftLen; i0 += n1) {
                /* Butterfly implementation */
                r1 = pSi0[0] + pSi2[0];
                r2 = pSi0[0] - pSi2[0];
                s1 = pSi0[1] + pSi2[1];
                s2 = pSi0[1] - pSi2[1];

                t1 = pSi1[0] + pSi3[0];

                pSi0[0] = (r1 + t1) >> 2u;
                r1 = r1 - t1;

                t2 = pSi1[1] + pSi3[1];

                pSi0[1] = (s1 + t2) >> 2u;
                pSi0 += 2 * n1;

                s1 = s1 - t2;

                t1 = pSi1[1] - pSi3[1];
                t2 = pSi1[0] - pSi3[0];

                pSi1[0] = (((int32_t)(((q63_t)r1 * co2) >> 32)) +
                            ((int32_t)(((q63_t)s1 * si2) >> 32))) >> 1u;

                pSi1[1] = (((int32_t)(((q63_t)s1 * co2) >> 32)) -
                            ((int32_t)(((q63_t)r1 * si2) >> 32))) >> 1u;
                pSi1 += 2 * n1;

                r1 = r2 + t1;
                r2 = r2 - t1;

                s1 = s2 - t2;
                s2 = s2 + t2;

                pSi2[0] = (((int32_t)(((q63_t)r1 * co1) >> 32)) +
                            ((int32_t)(((q63_t)s1 * si1) >> 32))) >> 1u;

                pSi2[1] = (((int32_t)(((q63_t)s1 * co1) >> 32)) -
                            ((int32_t)(((q63_t)r1 * si1) >> 32))) >> 1u;
                pSi2 += 2 * n1;

                pSi3[0] = (((int32_t)(((q63_t)r2 * co3) >> 32)) +
                            ((int32_t)(((q63_t)s2 * si3) >> 32))) >> 1u;

                pSi3[1] = (((int32_t)(((q63_t)s2 * co3) >> 32)) -
                            ((int32_t)(((q63_t)r2 * si3) >> 32))) >> 1u;
                pSi3 += 2 * n1;
            }
        }
        twidCoefModifier <<= 2u;
    }

    /* End of Middle stages process */

    /* start of Last stage process */
    j = fftLen >> 2;
    ptr1 = &pSrc[0];

    /* Calculations of last stage - use individual reads (no SIMD) */
    do {
        /* Read xa (real), ya(imag) input - individual reads */
        xa = ptr1[0];
        ya = ptr1[1];

        /* Read xb (real), yb(imag) input */
        xb = ptr1[2];
        yb = ptr1[3];

        /* Read xc (real), yc(imag) input */
        xc = ptr1[4];
        yc = ptr1[5];

        /* Read xd (real), yd(imag) input */
        xd = ptr1[6];
        yd = ptr1[7];

        /* xa' = xa + xb + xc + xd */
        xa_out = xa + xb + xc + xd;

        /* ya' = ya + yb + yc + yd */
        ya_out = ya + yb + yc + yd;

        /* xa' = xa + xb + xc + xd */
        *ptr1++ = xa_out;
        *ptr1++ = ya_out;

        xc_out = (xa - xb + xc - xd);
        yc_out = (ya - yb + yc - yd);

        /* writing xc' and yc' */
        *ptr1++ = xc_out;
        *ptr1++ = yc_out;

        xb_out = (xa + yb - xc - yd);
        yb_out = (ya - xb - yc + xd);

        /* writing xb' and yb' */
        *ptr1++ = xb_out;
        *ptr1++ = yb_out;

        xd_out = (xa - yb - xc + yd);
        yd_out = (ya + xb - yc - xd);

        /* writing xd' and yd' */
        *ptr1++ = xd_out;
        *ptr1++ = yd_out;

    } while (--j);
}

/* ----------------------------------------------------------------
 * arm_radix4_butterfly_inverse_q31 - non-CM7 path
 * ---------------------------------------------------------------- */
static void arm_radix4_butterfly_inverse_q31(
    q31_t *pSrc,
    uint32_t fftLen,
    q31_t *pCoef,
    uint32_t twidCoefModifier)
{
    /* non-CM7 path */
    uint32_t n1, n2, ia1, ia2, ia3, i0, j, k;
    q31_t t1, t2, r1, r2, s1, s2, co1, co2, co3, si1, si2, si3;
    q31_t xa, xb, xc, xd;
    q31_t ya, yb, yc, yd;
    q31_t xa_out, xb_out, xc_out, xd_out;
    q31_t ya_out, yb_out, yc_out, yd_out;

    q31_t *ptr1;
    q31_t *pSi0;
    q31_t *pSi1;
    q31_t *pSi2;
    q31_t *pSi3;

    /* Total process is divided into three stages */

    /* Start of first stage process */
    n2 = fftLen;
    n1 = n2;
    n2 >>= 2u;

    ia1 = 0u;

    j = n2;

    pSi0 = pSrc;
    pSi1 = pSi0 + 2 * n2;
    pSi2 = pSi1 + 2 * n2;
    pSi3 = pSi2 + 2 * n2;

    do {
        /* Butterfly implementation */
        r1 = (pSi0[0] >> 4u) + (pSi2[0] >> 4u);
        r2 = (pSi0[0] >> 4u) - (pSi2[0] >> 4u);

        t1 = (pSi1[0] >> 4u) + (pSi3[0] >> 4u);

        s1 = (pSi0[1] >> 4u) + (pSi2[1] >> 4u);
        s2 = (pSi0[1] >> 4u) - (pSi2[1] >> 4u);

        *pSi0++ = (r1 + t1);
        r1 = r1 - t1;

        t2 = (pSi1[1] >> 4u) + (pSi3[1] >> 4u);
        *pSi0++ = (s1 + t2);

        s1 = s1 - t2;

        t1 = (pSi1[1] >> 4u) - (pSi3[1] >> 4u);
        t2 = (pSi1[0] >> 4u) - (pSi3[0] >> 4u);

        ia2 = 2u * ia1;
        co2 = pCoef[ia2 * 2u];
        si2 = pCoef[(ia2 * 2u) + 1u];

        /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
        *pSi1++ = (((int32_t)(((q63_t)r1 * co2) >> 32)) -
                    ((int32_t)(((q63_t)s1 * si2) >> 32))) << 1u;

        /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
        *pSi1++ = (((int32_t)(((q63_t)s1 * co2) >> 32)) +
                    ((int32_t)(((q63_t)r1 * si2) >> 32))) << 1u;

        /* (xa - xc) - (yb - yd) */
        r1 = r2 - t1;
        /* (xa - xc) + (yb - yd) */
        r2 = r2 + t1;

        /* (ya - yc) + (xb - xd) */
        s1 = s2 + t2;
        /* (ya - yc) - (xb - xd) */
        s2 = s2 - t2;

        co1 = pCoef[ia1 * 2u];
        si1 = pCoef[(ia1 * 2u) + 1u];

        /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
        *pSi2++ = (((int32_t)(((q63_t)r1 * co1) >> 32)) -
                    ((int32_t)(((q63_t)s1 * si1) >> 32))) << 1u;

        /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
        *pSi2++ = (((int32_t)(((q63_t)s1 * co1) >> 32)) +
                    ((int32_t)(((q63_t)r1 * si1) >> 32))) << 1u;

        ia3 = 3u * ia1;
        co3 = pCoef[ia3 * 2u];
        si3 = pCoef[(ia3 * 2u) + 1u];

        /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
        *pSi3++ = (((int32_t)(((q63_t)r2 * co3) >> 32)) -
                    ((int32_t)(((q63_t)s2 * si3) >> 32))) << 1u;

        /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
        *pSi3++ = (((int32_t)(((q63_t)s2 * co3) >> 32)) +
                    ((int32_t)(((q63_t)r2 * si3) >> 32))) << 1u;

        ia1 = ia1 + twidCoefModifier;

    } while (--j);

    /* data is in 5.27(q27) format */

    /* Start of Middle stages process */
    twidCoefModifier <<= 2u;

    for (k = fftLen / 4u; k > 4u; k >>= 2u) {
        n1 = n2;
        n2 >>= 2u;
        ia1 = 0u;

        for (j = 0; j <= (n2 - 1u); j++) {
            ia2 = ia1 + ia1;
            ia3 = ia2 + ia1;
            co1 = pCoef[ia1 * 2u];
            si1 = pCoef[(ia1 * 2u) + 1u];
            co2 = pCoef[ia2 * 2u];
            si2 = pCoef[(ia2 * 2u) + 1u];
            co3 = pCoef[ia3 * 2u];
            si3 = pCoef[(ia3 * 2u) + 1u];
            ia1 = ia1 + twidCoefModifier;

            pSi0 = pSrc + 2 * j;
            pSi1 = pSi0 + 2 * n2;
            pSi2 = pSi1 + 2 * n2;
            pSi3 = pSi2 + 2 * n2;

            for (i0 = j; i0 < fftLen; i0 += n1) {
                /* Butterfly implementation */
                r1 = pSi0[0] + pSi2[0];
                r2 = pSi0[0] - pSi2[0];
                s1 = pSi0[1] + pSi2[1];
                s2 = pSi0[1] - pSi2[1];

                t1 = pSi1[0] + pSi3[0];

                pSi0[0] = (r1 + t1) >> 2u;
                r1 = r1 - t1;

                t2 = pSi1[1] + pSi3[1];

                pSi0[1] = (s1 + t2) >> 2u;
                pSi0 += 2 * n1;

                s1 = s1 - t2;

                t1 = pSi1[1] - pSi3[1];
                t2 = pSi1[0] - pSi3[0];

                /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
                pSi1[0] = (((int32_t)(((q63_t)r1 * co2) >> 32u)) -
                            ((int32_t)(((q63_t)s1 * si2) >> 32u))) >> 1u;

                /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
                pSi1[1] = (((int32_t)(((q63_t)s1 * co2) >> 32u)) +
                            ((int32_t)(((q63_t)r1 * si2) >> 32u))) >> 1u;
                pSi1 += 2 * n1;

                /* (xa - xc) - (yb - yd) */
                r1 = r2 - t1;
                /* (xa - xc) + (yb - yd) */
                r2 = r2 + t1;

                /* (ya - yc) + (xb - xd) */
                s1 = s2 + t2;
                /* (ya - yc) - (xb - xd) */
                s2 = s2 - t2;

                /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
                pSi2[0] = (((int32_t)(((q63_t)r1 * co1) >> 32)) -
                            ((int32_t)(((q63_t)s1 * si1) >> 32))) >> 1u;

                /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
                pSi2[1] = (((int32_t)(((q63_t)s1 * co1) >> 32)) +
                            ((int32_t)(((q63_t)r1 * si1) >> 32))) >> 1u;
                pSi2 += 2 * n1;

                /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
                pSi3[0] = (((int32_t)(((q63_t)r2 * co3) >> 32)) -
                            ((int32_t)(((q63_t)s2 * si3) >> 32))) >> 1u;

                /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
                pSi3[1] = (((int32_t)(((q63_t)s2 * co3) >> 32)) +
                            ((int32_t)(((q63_t)r2 * si3) >> 32))) >> 1u;
                pSi3 += 2 * n1;
            }
        }
        twidCoefModifier <<= 2u;
    }

    /* Start of last stage process */
    j = fftLen >> 2;
    ptr1 = &pSrc[0];

    /* Calculations of last stage - individual reads (no SIMD) */
    do {
        /* Read xa (real), ya(imag) input */
        xa = ptr1[0];
        ya = ptr1[1];

        /* Read xb (real), yb(imag) input */
        xb = ptr1[2];
        yb = ptr1[3];

        /* Read xc (real), yc(imag) input */
        xc = ptr1[4];
        yc = ptr1[5];

        /* Read xd (real), yd(imag) input */
        xd = ptr1[6];
        yd = ptr1[7];

        /* xa' = xa + xb + xc + xd */
        xa_out = xa + xb + xc + xd;

        /* ya' = ya + yb + yc + yd */
        ya_out = ya + yb + yc + yd;

        /* writing xa' and ya' */
        *ptr1++ = xa_out;
        *ptr1++ = ya_out;

        xc_out = (xa - xb + xc - xd);
        yc_out = (ya - yb + yc - yd);

        /* writing xc' and yc' */
        *ptr1++ = xc_out;
        *ptr1++ = yc_out;

        xb_out = (xa - yb - xc + yd);
        yb_out = (ya + xb - yc - xd);

        /* writing xb' and yb' */
        *ptr1++ = xb_out;
        *ptr1++ = yb_out;

        xd_out = (xa + yb - xc - yd);
        yd_out = (ya - xb - yc + xd);

        /* writing xd' and yd' */
        *ptr1++ = xd_out;
        *ptr1++ = yd_out;

    } while (--j);
}

/* ----------------------------------------------------------------
 * arm_cfft_radix4by2_q31
 * ---------------------------------------------------------------- */
static void arm_cfft_radix4by2_q31(
    q31_t *pSrc,
    uint32_t fftLen,
    const q31_t *pCoef)
{
    uint32_t i, l;
    uint32_t n2, ia;
    q31_t xt, yt, cosVal, sinVal;
    q31_t p0, p1;

    n2 = fftLen >> 1;
    ia = 0;
    for (i = 0; i < n2; i++) {
        cosVal = pCoef[2*ia];
        sinVal = pCoef[2*ia + 1];
        ia++;

        l = i + n2;
        xt = (pSrc[2 * i] >> 2) - (pSrc[2 * l] >> 2);
        pSrc[2 * i] = (pSrc[2 * i] >> 2) + (pSrc[2 * l] >> 2);

        yt = (pSrc[2 * i + 1] >> 2) - (pSrc[2 * l + 1] >> 2);
        pSrc[2 * i + 1] = (pSrc[2 * l + 1] >> 2) + (pSrc[2 * i + 1] >> 2);

        mult_32x32_keep32_R(p0, xt, cosVal);
        mult_32x32_keep32_R(p1, yt, cosVal);
        multAcc_32x32_keep32_R(p0, yt, sinVal);
        multSub_32x32_keep32_R(p1, xt, sinVal);

        pSrc[2u * l] = p0 << 1;
        pSrc[2u * l + 1u] = p1 << 1;
    }

    /* first col */
    arm_radix4_butterfly_q31(pSrc, n2, (q31_t*)pCoef, 2u);
    /* second col */
    arm_radix4_butterfly_q31(pSrc + fftLen, n2, (q31_t*)pCoef, 2u);

    for (i = 0; i < fftLen >> 1; i++) {
        p0 = pSrc[4*i+0];
        p1 = pSrc[4*i+1];
        xt = pSrc[4*i+2];
        yt = pSrc[4*i+3];

        p0 <<= 1;
        p1 <<= 1;
        xt <<= 1;
        yt <<= 1;

        pSrc[4*i+0] = p0;
        pSrc[4*i+1] = p1;
        pSrc[4*i+2] = xt;
        pSrc[4*i+3] = yt;
    }
}

/* ----------------------------------------------------------------
 * arm_cfft_radix4by2_inverse_q31
 * ---------------------------------------------------------------- */
static void arm_cfft_radix4by2_inverse_q31(
    q31_t *pSrc,
    uint32_t fftLen,
    const q31_t *pCoef)
{
    uint32_t i, l;
    uint32_t n2, ia;
    q31_t xt, yt, cosVal, sinVal;
    q31_t p0, p1;

    n2 = fftLen >> 1;
    ia = 0;
    for (i = 0; i < n2; i++) {
        cosVal = pCoef[2*ia];
        sinVal = pCoef[2*ia + 1];
        ia++;

        l = i + n2;
        xt = (pSrc[2 * i] >> 2) - (pSrc[2 * l] >> 2);
        pSrc[2 * i] = (pSrc[2 * i] >> 2) + (pSrc[2 * l] >> 2);

        yt = (pSrc[2 * i + 1] >> 2) - (pSrc[2 * l + 1] >> 2);
        pSrc[2 * i + 1] = (pSrc[2 * l + 1] >> 2) + (pSrc[2 * i + 1] >> 2);

        mult_32x32_keep32_R(p0, xt, cosVal);
        mult_32x32_keep32_R(p1, yt, cosVal);
        multSub_32x32_keep32_R(p0, yt, sinVal);
        multAcc_32x32_keep32_R(p1, xt, sinVal);

        pSrc[2u * l] = p0 << 1;
        pSrc[2u * l + 1u] = p1 << 1;
    }

    /* first col */
    arm_radix4_butterfly_inverse_q31(pSrc, n2, (q31_t*)pCoef, 2u);
    /* second col */
    arm_radix4_butterfly_inverse_q31(pSrc + fftLen, n2, (q31_t*)pCoef, 2u);

    for (i = 0; i < fftLen >> 1; i++) {
        p0 = pSrc[4*i+0];
        p1 = pSrc[4*i+1];
        xt = pSrc[4*i+2];
        yt = pSrc[4*i+3];

        p0 <<= 1;
        p1 <<= 1;
        xt <<= 1;
        yt <<= 1;

        pSrc[4*i+0] = p0;
        pSrc[4*i+1] = p1;
        pSrc[4*i+2] = xt;
        pSrc[4*i+3] = yt;
    }
}

/* ----------------------------------------------------------------
 * arm_cfft_q31 - Main entry point
 * ---------------------------------------------------------------- */
void arm_cfft_q31(
    const arm_cfft_instance_q31 *S,
    q31_t *p1,
    uint8_t ifftFlag,
    uint8_t bitReverseFlag)
{
    uint32_t L = S->fftLen;

    if (ifftFlag == 1u) {
        switch (L) {
        case 16:
        case 64:
        case 256:
        case 1024:
        case 4096:
            arm_radix4_butterfly_inverse_q31(p1, L, (q31_t*)S->pTwiddle, 1);
            break;
        case 32:
        case 128:
        case 512:
        case 2048:
            arm_cfft_radix4by2_inverse_q31(p1, L, S->pTwiddle);
            break;
        }
    } else {
        switch (L) {
        case 16:
        case 64:
        case 256:
        case 1024:
        case 4096:
            arm_radix4_butterfly_q31(p1, L, (q31_t*)S->pTwiddle, 1);
            break;
        case 32:
        case 128:
        case 512:
        case 2048:
            arm_cfft_radix4by2_q31(p1, L, S->pTwiddle);
            break;
        }
    }

    if (bitReverseFlag)
        arm_bitreversal_32((uint32_t*)p1, S->bitRevLength, S->pBitRevTable);
}