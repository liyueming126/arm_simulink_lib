#ifndef CFFT_Q15_SIMULINK_H
#define CFFT_Q15_SIMULINK_H

#include <stdint.h>

typedef int16_t  q15_t;
typedef uint32_t uint32_t;
typedef int32_t  q31_t;
typedef uint8_t  uint8_t;

typedef struct{
  uint32_t fftLen;
  const q15_t *pTwiddle;
  uint16_t bitRevLength;
  const uint16_t *pBitRevTable;
}arm_cfft_instance_q15;

#define _SIMD32_OFFSET(x) (*((q31_t*)(x)))

static inline q31_t __SHADD16(q31_t val,q31_t dummy)
{
    int16_t hh = (int16_t)((val>>16)&0xFFFF);
    int16_t ll = (int16_t)(val&0xFFFF);
    hh >>=1; ll>>=1;
    return ((q31_t)hh<<16)|((q31_t)ll&0xFFFF);
}
static inline q31_t __QSUB16(q31_t a,q31_t b)
{
    int16_t ah=(int16_t)(a>>16),al=(int16_t)(a&0xFFFF);
    int16_t bh=(int16_t)(b>>16),bl=(int16_t)(b&0xFFFF);
    return ((q31_t)(ah-bh)<<16)|((q31_t)(al-bl)&0xFFFF);
}
static inline q31_t __SMUAD(q31_t coeff,q31_t R)
{
    int16_t c0=(int16_t)(coeff>>16),c1=(int16_t)(coeff&0xFFFF);
    int16_t r0=(int16_t)(R>>16),r1=(int16_t)(R&0xFFFF);
    return (q31_t)c0*r0 + (q31_t)c1*r1;
}
static inline q31_t __SMUSDX(q31_t coeff,q31_t R)
{
    int16_t c0=(int16_t)(coeff>>16),c1=(int16_t)(coeff&0xFFFF);
    int16_t r0=(int16_t)(R>>16),r1=(int16_t)(R&0xFFFF);
    return (q31_t)c0*r1 - (q31_t)c1*r0;
}
static inline q31_t __SMUSD(q31_t coeff,q31_t R)
{
    int16_t c0=(int16_t)(coeff>>16),c1=(int16_t)(coeff&0xFFFF);
    int16_t r0=(int16_t)(R>>16),r1=(int16_t)(R&0xFFFF);
    return (q31_t)c0*r0 - (q31_t)c1*r1;
}
static inline q31_t __SMUADX(q31_t coeff,q31_t R)
{
    int16_t c0=(int16_t)(coeff>>16),c1=(int16_t)(coeff&0xFFFF);
    int16_t r0=(int16_t)(R>>16),r1=(int16_t)(R&0xFFFF);
    return (q31_t)c0*r1 + (q31_t)c1*r0;
}
static inline q31_t __QSUB(q31_t a,q31_t b)
{
    return a-b;
}

extern void arm_radix4_butterfly_q15(q15_t * pSrc,uint32_t fftLen,q15_t * pCoef,uint32_t twidCoefModifier);
extern void arm_radix4_butterfly_inverse_q15(q15_t * pSrc,uint32_t fftLen,q15_t * pCoef,uint32_t twidCoefModifier);
extern void arm_bitreversal_16(uint16_t * pSrc,const uint16_t bitRevLen,const uint16_t * pBitRevTable);
void arm_cfft_radix4by2_q15(q15_t * pSrc,uint32_t fftLen,const q15_t * pCoef);
void arm_cfft_radix4by2_inverse_q15(q15_t * pSrc,uint32_t fftLen,const q15_t * pCoef);
void arm_cfft_q15(const arm_cfft_instance_q15 * S,q15_t * p1,uint8_t ifftFlag,uint8_t bitReverseFlag);
void cfft_q15_wrapper(uint32_t fftLen,uint8_t ifftFlag,uint8_t bitRevFlag,q15_t *inBuf,q15_t *outBuf);

extern const q15_t twiddleTable16[];
extern const uint16_t bitRevTable16[];
extern const q15_t twiddleTable32[];
extern const uint16_t bitRevTable32[];

#endif