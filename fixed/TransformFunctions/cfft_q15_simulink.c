#include "cfft_q15_simulink.h"

const q15_t twiddleTable16[] = {
    32767, 0, 30272, -12539, 23170, -23170, 12539, -30272,
    0, -32767, -12539, -30272, -23170, -23170, -30272, -12539
};
const uint16_t bitRevTable16[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};

const q15_t twiddleTable32[] = {
    32767, 0, 32139, -6392, 30272, -12539, 27244, -18205,
    23170, -23170, 18192, -27244, 12539, -30272, 6392, -32139,
    0, -32767, -6392, -32139, -12539, -30272, -18205, -27244,
    -23170, -23170, -27244, -18205, -30272, -12539, -32139, -6392
};
const uint16_t bitRevTable32[] = {
    0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30,
    1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31
};

void arm_bitreversal_16(uint16_t *pSrc, const uint16_t bitRevLen, const uint16_t *pBitRevTable)
{
    uint32_t i;
    uint16_t tmp;
    for (i = 0; i < bitRevLen; i++)
    {
        uint16_t idx = pBitRevTable[i];
        if (idx > i)
        {
            tmp = pSrc[2 * i];
            pSrc[2 * i] = pSrc[2 * idx];
            pSrc[2 * idx] = tmp;
            tmp = pSrc[2 * i + 1];
            pSrc[2 * i + 1] = pSrc[2 * idx + 1];
            pSrc[2 * idx + 1] = tmp;
        }
    }
}

/* 正向 Radix-4 蝶形 - 参考官方 CMSIS-DSP 实现 */
void arm_radix4_butterfly_q15(q15_t *pSrc, uint32_t fftLen, q15_t *pCoef, uint32_t twidCoefModifier)
{
    uint32_t n1, n2, ia1, ia2, ia3;
    uint32_t i0, i1, i2, i3;
    uint32_t j, k;
    q15_t t1r, t1i, t2r, t2i, t3r, t3i, t4r, t4i;
    q31_t x1r, x1i, x2r, x2i, x3r, x3i;
    q15_t co1, si1, co2, si2, co3, si3;

    /* 初始化 */
    n2 = fftLen;
    n1 = n2;
    
    /* n2 为每组 butterfly 的间距，n1 为每组内的 butterfly 数量 */
    while (n2 > 4)
    {
        /* 第 1 级: n1=4, n2=16, 每组 4 个 butterfly，间距 16 */
        /* 第 2 级: n1=1, n2=4,  每组 1 个 butterfly，间距 4  */
        n1 = n2;
        n2 >>= 2;
        
        ia1 = 0;
        
        for (j = 0; j < n2; j++)
        {
            ia2 = ia1 + ia1;
            ia3 = ia2 + ia1;
            
            co1 = pCoef[ia1 * 2];
            si1 = pCoef[(ia1 * 2) + 1];
            co2 = pCoef[ia2 * 2];
            si2 = pCoef[(ia2 * 2) + 1];
            co3 = pCoef[ia3 * 2];
            si3 = pCoef[(ia3 * 2) + 1];
            
            ia1 += twidCoefModifier;
            
            for (k = j; k < fftLen; k += n1)
            {
                /* 计算 4 个点的索引 */
                i0 = k * 2;
                i1 = i0 + n2 * 2;
                i2 = i1 + n2 * 2;
                i3 = i2 + n2 * 2;
                
                /* 读取 4 个点 */
                t1r = pSrc[i0];
                t1i = pSrc[i0 + 1];
                t2r = pSrc[i1];
                t2i = pSrc[i1 + 1];
                t3r = pSrc[i2];
                t3i = pSrc[i2 + 1];
                t4r = pSrc[i3];
                t4i = pSrc[i3 + 1];
                
                /* x1 = t2 * twiddle1 */
                x1r = ((q31_t)t2r * co1) - ((q31_t)t2i * si1);
                x1i = ((q31_t)t2r * si1) + ((q31_t)t2i * co1);
                t2r = (q15_t)(x1r >> 15);
                t2i = (q15_t)(x1i >> 15);
                
                /* x2 = t3 * twiddle2 */
                x2r = ((q31_t)t3r * co2) - ((q31_t)t3i * si2);
                x2i = ((q31_t)t3r * si2) + ((q31_t)t3i * co2);
                t3r = (q15_t)(x2r >> 15);
                t3i = (q15_t)(x2i >> 15);
                
                /* x3 = t4 * twiddle3 */
                x3r = ((q31_t)t4r * co3) - ((q31_t)t4i * si3);
                x3i = ((q31_t)t4r * si3) + ((q31_t)t4i * co3);
                t4r = (q15_t)(x3r >> 15);
                t4i = (q15_t)(x3i >> 15);
                
                /* Radix-4 蝶形运算 */
                /* X[k]   = t1 + t2 + t3 + t4 */
                pSrc[i0]     = (q15_t)(((q31_t)t1r + t2r + t3r + t4r) >> 2);
                pSrc[i0 + 1] = (q15_t)(((q31_t)t1i + t2i + t3i + t4i) >> 2);
                
                /* X[k+n2]   = t1 + j*t2 - t3 - j*t4 */
                pSrc[i1]     = (q15_t)(((q31_t)t1r + t2i - t3r - t4i) >> 2);
                pSrc[i1 + 1] = (q15_t)(((q31_t)t1i - t2r - t3i + t4r) >> 2);
                
                /* X[k+2*n2] = t1 - t2 + t3 - t4 */
                pSrc[i2]     = (q15_t)(((q31_t)t1r - t2r + t3r - t4r) >> 2);
                pSrc[i2 + 1] = (q15_t)(((q31_t)t1i - t2i + t3i - t4i) >> 2);
                
                /* X[k+3*n2] = t1 - j*t2 - t3 + j*t4 */
                pSrc[i3]     = (q15_t)(((q31_t)t1r - t2i - t3r + t4i) >> 2);
                pSrc[i3 + 1] = (q15_t)(((q31_t)t1i + t2r - t3i - t4r) >> 2);
            }
        }
        
        twidCoefModifier <<= 2;  /* 下一级 twiddle 步进增大 4 倍 */
    }
    
    /* 最后 n2=4 的一级，无 twiddle（co=1, si=0） */
    n1 = n2;
    n2 >>= 2;  /* n2 = 1 */
    
    for (k = 0; k < fftLen; k += n1)
    {
        i0 = k * 2;
        i1 = i0 + 2;
        i2 = i1 + 2;
        i3 = i2 + 2;
        
        t1r = pSrc[i0];
        t1i = pSrc[i0 + 1];
        t2r = pSrc[i1];
        t2i = pSrc[i1 + 1];
        t3r = pSrc[i2];
        t3i = pSrc[i2 + 1];
        t4r = pSrc[i3];
        t4i = pSrc[i3 + 1];
        
        pSrc[i0]     = (q15_t)(((q31_t)t1r + t2r + t3r + t4r) >> 2);
        pSrc[i0 + 1] = (q15_t)(((q31_t)t1i + t2i + t3i + t4i) >> 2);
        
        pSrc[i1]     = (q15_t)(((q31_t)t1r + t2i - t3r - t4i) >> 2);
        pSrc[i1 + 1] = (q15_t)(((q31_t)t1i - t2r - t3i + t4r) >> 2);
        
        pSrc[i2]     = (q15_t)(((q31_t)t1r - t2r + t3r - t4r) >> 2);
        pSrc[i2 + 1] = (q15_t)(((q31_t)t1i - t2i + t3i - t4i) >> 2);
        
        pSrc[i3]     = (q15_t)(((q31_t)t1r - t2i - t3r + t4i) >> 2);
        pSrc[i3 + 1] = (q15_t)(((q31_t)t1i + t2r - t3i - t4r) >> 2);
    }
}

/* 逆向 Radix-4 蝶形 */
void arm_radix4_butterfly_inverse_q15(q15_t *pSrc, uint32_t fftLen, q15_t *pCoef, uint32_t twidCoefModifier)
{
    uint32_t n1, n2, ia1, ia2, ia3;
    uint32_t i0, i1, i2, i3;
    uint32_t j, k;
    q15_t t1r, t1i, t2r, t2i, t3r, t3i, t4r, t4i;
    q31_t x1r, x1i, x2r, x2i, x3r, x3i;
    q15_t co1, si1, co2, si2, co3, si3;

    n2 = fftLen;
    n1 = n2;
    
    while (n2 > 4)
    {
        n1 = n2;
        n2 >>= 2;
        
        ia1 = 0;
        
        for (j = 0; j < n2; j++)
        {
            ia2 = ia1 + ia1;
            ia3 = ia2 + ia1;
            
            /* IFFT: sin 取负（共轭 twiddle） */
            co1 = pCoef[ia1 * 2];
            si1 = -pCoef[(ia1 * 2) + 1];
            co2 = pCoef[ia2 * 2];
            si2 = -pCoef[(ia2 * 2) + 1];
            co3 = pCoef[ia3 * 2];
            si3 = -pCoef[(ia3 * 2) + 1];
            
            ia1 += twidCoefModifier;
            
            for (k = j; k < fftLen; k += n1)
            {
                i0 = k * 2;
                i1 = i0 + n2 * 2;
                i2 = i1 + n2 * 2;
                i3 = i2 + n2 * 2;
                
                t1r = pSrc[i0];
                t1i = pSrc[i0 + 1];
                t2r = pSrc[i1];
                t2i = pSrc[i1 + 1];
                t3r = pSrc[i2];
                t3i = pSrc[i2 + 1];
                t4r = pSrc[i3];
                t4i = pSrc[i3 + 1];
                
                x1r = ((q31_t)t2r * co1) - ((q31_t)t2i * si1);
                x1i = ((q31_t)t2r * si1) + ((q31_t)t2i * co1);
                t2r = (q15_t)(x1r >> 15);
                t2i = (q15_t)(x1i >> 15);
                
                x2r = ((q31_t)t3r * co2) - ((q31_t)t3i * si2);
                x2i = ((q31_t)t3r * si2) + ((q31_t)t3i * co2);
                t3r = (q15_t)(x2r >> 15);
                t3i = (q15_t)(x2i >> 15);
                
                x3r = ((q31_t)t4r * co3) - ((q31_t)t4i * si3);
                x3i = ((q31_t)t4r * si3) + ((q31_t)t4i * co3);
                t4r = (q15_t)(x3r >> 15);
                t4i = (q15_t)(x3i >> 15);
                
                /* IFFT 蝶形（注意符号变化） */
                pSrc[i0]     = (q15_t)(((q31_t)t1r + t2r + t3r + t4r) >> 2);
                pSrc[i0 + 1] = (q15_t)(((q31_t)t1i + t2i + t3i + t4i) >> 2);
                
                pSrc[i1]     = (q15_t)(((q31_t)t1r - t2i - t3r + t4i) >> 2);
                pSrc[i1 + 1] = (q15_t)(((q31_t)t1i + t2r - t3i - t4r) >> 2);
                
                pSrc[i2]     = (q15_t)(((q31_t)t1r - t2r + t3r - t4r) >> 2);
                pSrc[i2 + 1] = (q15_t)(((q31_t)t1i - t2i + t3i - t4i) >> 2);
                
                pSrc[i3]     = (q15_t)(((q31_t)t1r + t2i - t3r - t4i) >> 2);
                pSrc[i3 + 1] = (q15_t)(((q31_t)t1i - t2r - t3i + t4r) >> 2);
            }
        }
        
        twidCoefModifier <<= 2;
    }
    
    /* 最后一级无 twiddle */
    n1 = n2;
    n2 >>= 2;
    
    for (k = 0; k < fftLen; k += n1)
    {
        i0 = k * 2;
        i1 = i0 + 2;
        i2 = i1 + 2;
        i3 = i2 + 2;
        
        t1r = pSrc[i0];
        t1i = pSrc[i0 + 1];
        t2r = pSrc[i1];
        t2i = pSrc[i1 + 1];
        t3r = pSrc[i2];
        t3i = pSrc[i2 + 1];
        t4r = pSrc[i3];
        t4i = pSrc[i3 + 1];
        
        pSrc[i0]     = (q15_t)(((q31_t)t1r + t2r + t3r + t4r) >> 2);
        pSrc[i0 + 1] = (q15_t)(((q31_t)t1i + t2i + t3i + t4i) >> 2);
        
        pSrc[i1]     = (q15_t)(((q31_t)t1r - t2i - t3r + t4i) >> 2);
        pSrc[i1 + 1] = (q15_t)(((q31_t)t1i + t2r - t3i - t4r) >> 2);
        
        pSrc[i2]     = (q15_t)(((q31_t)t1r - t2r + t3r - t4r) >> 2);
        pSrc[i2 + 1] = (q15_t)(((q31_t)t1i - t2i + t3i - t4i) >> 2);
        
        pSrc[i3]     = (q15_t)(((q31_t)t1r + t2i - t3r - t4i) >> 2);
        pSrc[i3 + 1] = (q15_t)(((q31_t)t1i - t2r - t3i + t4r) >> 2);
    }
}

/* 官方实现：FFT 后做位反转 */
void arm_cfft_q15(const arm_cfft_instance_q15 *S, q15_t *p1, uint8_t ifftFlag, uint8_t bitReverseFlag)
{
    uint32_t L = S->fftLen;

    if (ifftFlag == 1u)
    {
        switch (L)
        {
        case 16:
        case 64:
        case 256:
        case 1024:
        case 4096:
            arm_radix4_butterfly_inverse_q15(p1, L, (q15_t *)S->pTwiddle, 1);
            break;
            
        case 32:
        case 128:
        case 512:
        case 2048:
            arm_cfft_radix4by2_inverse_q15(p1, L, S->pTwiddle);
            break;
        }
    }
    else
    {
        switch (L)
        {
        case 16:
        case 64:
        case 256:
        case 1024:
        case 4096:
            arm_radix4_butterfly_q15(p1, L, (q15_t *)S->pTwiddle, 1);
            break;
            
        case 32:
        case 128:
        case 512:
        case 2048:
            arm_cfft_radix4by2_q15(p1, L, S->pTwiddle);
            break;
        }
    }
    
    if (bitReverseFlag)
        arm_bitreversal_16((uint16_t *)p1, S->bitRevLength, S->pBitRevTable);
}

/* Radix-4 by 2 正向（32/128/512/2048 点） */
void arm_cfft_radix4by2_q15(q15_t *pSrc, uint32_t fftLen, const q15_t *pCoef)
{
    uint32_t i;
    uint32_t n2;
    q15_t p0, p1, p2, p3;
    
    q31_t T, S, R;
    q31_t coeff, out1, out2;
    const q15_t *pC = pCoef;
    q15_t *pSi = pSrc;
    q15_t *pSl = pSrc + fftLen;
    
    n2 = fftLen >> 1;

    for (i = n2; i > 0; i--)
    {
        coeff = _SIMD32_OFFSET(pC);
        pC += 2;

        T = _SIMD32_OFFSET(pSi);
        T = __SHADD16(T, 0);

        S = _SIMD32_OFFSET(pSl);
        S = __SHADD16(S, 0);

        R = __QSUB16(T, S);

        _SIMD32_OFFSET(pSi) = __SHADD16(T, S);
        pSi += 2;

        out1 = __SMUAD(coeff, R) >> 16;
        out2 = __SMUSDX(coeff, R);

        _SIMD32_OFFSET(pSl) = (q31_t)((out2 & 0xFFFF0000) | (out1 & 0x0000FFFF));
        pSl += 2;
    }
    
    /* first col */
    arm_radix4_butterfly_q15(pSrc, n2, (q15_t *)pCoef, 2u);
    /* second col */
    arm_radix4_butterfly_q15(pSrc + fftLen, n2, (q15_t *)pCoef, 2u);
    
    for (i = 0; i < (fftLen >> 1); i++)
    {
        p0 = pSrc[4 * i + 0];
        p1 = pSrc[4 * i + 1];
        p2 = pSrc[4 * i + 2];
        p3 = pSrc[4 * i + 3];
        
        p0 <<= 1;
        p1 <<= 1;
        p2 <<= 1;
        p3 <<= 1;
        
        pSrc[4 * i + 0] = p0;
        pSrc[4 * i + 1] = p1;
        pSrc[4 * i + 2] = p2;
        pSrc[4 * i + 3] = p3;
    }
}

/* Radix-4 by 2 逆向 */
void arm_cfft_radix4by2_inverse_q15(q15_t *pSrc, uint32_t fftLen, const q15_t *pCoef)
{
    uint32_t i;
    uint32_t n2;
    q15_t p0, p1, p2, p3;
    
    q31_t T, S, R;
    q31_t coeff, out1, out2;
    const q15_t *pC = pCoef;
    q15_t *pSi = pSrc;
    q15_t *pSl = pSrc + fftLen;
    
    n2 = fftLen >> 1;

    for (i = n2; i > 0; i--)
    {
        coeff = _SIMD32_OFFSET(pC);
        pC += 2;

        T = _SIMD32_OFFSET(pSi);
        T = __SHADD16(T, 0);

        S = _SIMD32_OFFSET(pSl);
        S = __SHADD16(S, 0);

        R = __QSUB16(T, S);

        _SIMD32_OFFSET(pSi) = __SHADD16(T, S);
        pSi += 2;

        out1 = __SMUSD(coeff, R) >> 16;
        out2 = __SMUADX(coeff, R);

        _SIMD32_OFFSET(pSl) = (q31_t)((out2 & 0xFFFF0000) | (out1 & 0x0000FFFF));
        pSl += 2;
    }
    
    /* first col */
    arm_radix4_butterfly_inverse_q15(pSrc, n2, (q15_t *)pCoef, 2u);
    /* second col */
    arm_radix4_butterfly_inverse_q15(pSrc + fftLen, n2, (q15_t *)pCoef, 2u);
    
    for (i = 0; i < (fftLen >> 1); i++)
    {
        p0 = pSrc[4 * i + 0];
        p1 = pSrc[4 * i + 1];
        p2 = pSrc[4 * i + 2];
        p3 = pSrc[4 * i + 3];
        
        p0 <<= 1;
        p1 <<= 1;
        p2 <<= 1;
        p3 <<= 1;
        
        pSrc[4 * i + 0] = p0;
        pSrc[4 * i + 1] = p1;
        pSrc[4 * i + 2] = p2;
        pSrc[4 * i + 3] = p3;
    }
}

void cfft_q15_wrapper(uint32_t fftLen, uint8_t ifftFlag, uint8_t bitRevFlag, q15_t *inBuf, q15_t *outBuf)
{
    arm_cfft_instance_q15 inst;
    uint32_t bufLen = fftLen * 2U;
    switch (fftLen)
    {
    case 16U:
        inst.pTwiddle = twiddleTable16;
        inst.pBitRevTable = bitRevTable16;
        inst.bitRevLength = 16U;
        break;
    case 32U:
        inst.pTwiddle = twiddleTable32;
        inst.pBitRevTable = bitRevTable32;
        inst.bitRevLength = 32U;
        break;
    default:
        inst.pTwiddle = NULL;
        inst.pBitRevTable = NULL;
        inst.bitRevLength = 0;
        break;
    }
    inst.fftLen = fftLen;
    for (uint32_t i = 0; i < bufLen; i++)
        outBuf[i] = inBuf[i];
    arm_cfft_q15(&inst, outBuf, ifftFlag, bitRevFlag);
}