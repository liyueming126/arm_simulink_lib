/* ----------------------------------------------------------------------
 * arm_dct4_f32_simulink.c
 * Self-contained DCT4/IDCT4 for Simulink.
 * Based on CMSIS arm_dct4_f32.c (CM0 path) with embedded RFFT.
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* DCT4 instance structure */
typedef struct {
    uint16_t N;
    uint16_t Nby2;
    float normalize;
    float *pTwiddle;
    float *pCosFactor;
    void *pRfft;
} arm_dct4_instance_f32;

/* RFFT instance (deprecated) */
typedef struct {
    uint32_t fftLenReal;
    uint32_t fftLenBy2;
    uint8_t ifftFlagR;
    uint8_t bitReverseFlagR;
    uint32_t twidCoefRModifier;
    float *pTwiddleAReal;
    float *pTwiddleBReal;
    void *pCfft;
} arm_rfft_instance_f32;

/* CFFT instance */
typedef struct {
    uint16_t fftLen;
    uint8_t ifftFlag;
    uint8_t bitReverseFlag;
    float *pTwiddle;
    uint16_t *pBitRevTable;
    uint16_t twidCoefModifier;
    uint16_t bitRevFactor;
    float onebyfftLen;
} arm_cfft_radix4_instance_f32;

/* ----------------------------------------------------------------
 * Helper functions
 * ---------------------------------------------------------------- */
static void arm_scale_f32(float *pSrc, float scale, float *pDst, uint32_t n)
{
    uint32_t i;
    for (i = 0; i < n; i++) pDst[i] = pSrc[i] * scale;
}

static void arm_mult_f32(float *pSrcA, float *pSrcB, float *pDst, uint32_t n)
{
    uint32_t i;
    for (i = 0; i < n; i++) pDst[i] = pSrcA[i] * pSrcB[i];
}

static void arm_cmplx_mult_cmplx_f32(float *pSrcA, float *pSrcB, float *pDst, uint32_t n)
{
    uint32_t i;
    for (i = 0; i < n; i++) {
        float aR = pSrcA[2*i], aI = pSrcA[2*i+1];
        float bR = pSrcB[2*i], bI = pSrcB[2*i+1];
        pDst[2*i]   = aR * bR - aI * bI;
        pDst[2*i+1] = aI * bR + aR * bI;
    }
}

/* ----------------------------------------------------------------
 * RFFT components (radix-4 butterfly, split, bit reversal)
 * ---------------------------------------------------------------- */
static void radix4_bf_f32(float *pSrc, uint16_t fftLen, float *pCoef, uint16_t twidMod)
{
  float t1,t2,r1,r2,s1,s2,co1,co2,co3,si1,si2,si3;
  uint32_t ia1,ia2,ia3,i0,i1,i2,i3,n1,n2,j,k;
  n2=fftLen; n1=n2;
  for(k=fftLen;k>1u;k>>=2u){n1=n2;n2>>=2u;ia1=0u;j=0;
  do{ia2=ia1+ia1;ia3=ia2+ia1;
    co1=pCoef[ia1*2u];si1=pCoef[ia1*2u+1];co2=pCoef[ia2*2u];si2=pCoef[ia2*2u+1];
    co3=pCoef[ia3*2u];si3=pCoef[ia3*2u+1];ia1+=twidMod;i0=j;
    do{i1=i0+n2;i2=i1+n2;i3=i2+n2;
      r1=pSrc[2*i0]+pSrc[2*i2];r2=pSrc[2*i0]-pSrc[2*i2];
      s1=pSrc[2*i0+1]+pSrc[2*i2+1];s2=pSrc[2*i0+1]-pSrc[2*i2+1];
      t1=pSrc[2*i1]+pSrc[2*i3];pSrc[2*i0]=r1+t1;r1=r1-t1;
      t2=pSrc[2*i1+1]+pSrc[2*i3+1];pSrc[2*i0+1]=s1+t2;s1=s1-t2;
      t1=pSrc[2*i1+1]-pSrc[2*i3+1];t2=pSrc[2*i1]-pSrc[2*i3];
      pSrc[2*i1]=(r1*co2)+(s1*si2);pSrc[2*i1+1]=(s1*co2)-(r1*si2);
      r1=r2+t1;r2=r2-t1;s1=s2-t2;s2=s2+t2;
      pSrc[2*i2]=(r1*co1)+(s1*si1);pSrc[2*i2+1]=(s1*co1)-(r1*si1);
      pSrc[2*i3]=(r2*co3)+(s2*si3);pSrc[2*i3+1]=(s2*co3)-(r2*si3);
      i0+=n1;}while(i0<fftLen);j++;}while(j<=(n2-1u));twidMod<<=2u;}
}

static void radix4_bf_inv_f32(float *pSrc, uint16_t fftLen, float *pCoef, uint16_t twidMod, float onebyfftLen)
{
  float t1,t2,r1,r2,s1,s2,co1,co2,co3,si1,si2,si3;
  uint32_t ia1,ia2,ia3,i0,i1,i2,i3,n1,n2,j,k;
  n2=fftLen;n1=n2;
  for(k=fftLen;k>4u;k>>=2u){n1=n2;n2>>=2u;ia1=0u;j=0;
  do{ia2=ia1+ia1;ia3=ia2+ia1;
    co1=pCoef[ia1*2u];si1=pCoef[ia1*2u+1];co2=pCoef[ia2*2u];si2=pCoef[ia2*2u+1];
    co3=pCoef[ia3*2u];si3=pCoef[ia3*2u+1];ia1+=twidMod;i0=j;
    do{i1=i0+n2;i2=i1+n2;i3=i2+n2;
      r1=pSrc[2*i0]+pSrc[2*i2];r2=pSrc[2*i0]-pSrc[2*i2];
      s1=pSrc[2*i0+1]+pSrc[2*i2+1];s2=pSrc[2*i0+1]-pSrc[2*i2+1];
      t1=pSrc[2*i1]+pSrc[2*i3];pSrc[2*i0]=r1+t1;r1=r1-t1;
      t2=pSrc[2*i1+1]+pSrc[2*i3+1];pSrc[2*i0+1]=s1+t2;s1=s1-t2;
      t1=pSrc[2*i1+1]-pSrc[2*i3+1];t2=pSrc[2*i1]-pSrc[2*i3];
      pSrc[2*i1]=(r1*co2)-(s1*si2);pSrc[2*i1+1]=(s1*co2)+(r1*si2);
      r1=r2-t1;r2=r2+t1;s1=s2+t2;s2=s2-t2;
      pSrc[2*i2]=(r1*co1)-(s1*si1);pSrc[2*i2+1]=(s1*co1)+(r1*si1);
      pSrc[2*i3]=(r2*co3)-(s2*si3);pSrc[2*i3+1]=(s2*co3)+(r2*si3);
      i0+=n1;}while(i0<fftLen);j++;}while(j<=(n2-1u));twidMod<<=2u;}
  n1=n2;n2>>=2u;
  for(i0=0u;i0<=(fftLen-n1);i0+=n1){
    i1=i0+n2;i2=i1+n2;i3=i2+n2;
    r1=pSrc[2*i0]+pSrc[2*i2];r2=pSrc[2*i0]-pSrc[2*i2];
    s1=pSrc[2*i0+1]+pSrc[2*i2+1];s2=pSrc[2*i0+1]-pSrc[2*i2+1];
    t1=pSrc[2*i1]+pSrc[2*i3];pSrc[2*i0]=(r1+t1)*onebyfftLen;r1=r1-t1;
    t2=pSrc[2*i1+1]+pSrc[2*i3+1];pSrc[2*i0+1]=(s1+t2)*onebyfftLen;s1=s1-t2;
    t1=pSrc[2*i1+1]-pSrc[2*i3+1];t2=pSrc[2*i1]-pSrc[2*i3];
    pSrc[2*i1]=r1*onebyfftLen;pSrc[2*i1+1]=s1*onebyfftLen;
    r1=r2-t1;r2=r2+t1;s1=s2+t2;s2=s2-t2;
    pSrc[2*i2]=r1*onebyfftLen;pSrc[2*i2+1]=s1*onebyfftLen;
    pSrc[2*i3]=r2*onebyfftLen;pSrc[2*i3+1]=s2*onebyfftLen;}
}

static void bitrev_f32_rf(float *pSrc, uint16_t fftSize, uint16_t bitRevFactor, uint16_t *pBitRevTab)
{
  uint16_t fftLenBy2,fftLenBy2p1,i,j;float in;
  j=0u;fftLenBy2=fftSize>>1u;fftLenBy2p1=(fftSize>>1u)+1u;
  for(i=0u;i<=(fftLenBy2-2u);i+=2u){
    if(i<j){
      in=pSrc[2u*i];pSrc[2u*i]=pSrc[2u*j];pSrc[2u*j]=in;
      in=pSrc[2u*i+1];pSrc[2u*i+1]=pSrc[2u*j+1];pSrc[2u*j+1]=in;
      in=pSrc[2u*(i+fftLenBy2p1)];pSrc[2u*(i+fftLenBy2p1)]=pSrc[2u*(j+fftLenBy2p1)];pSrc[2u*(j+fftLenBy2p1)]=in;
      in=pSrc[2u*(i+fftLenBy2p1)+1];pSrc[2u*(i+fftLenBy2p1)+1]=pSrc[2u*(j+fftLenBy2p1)+1];pSrc[2u*(j+fftLenBy2p1)+1]=in;
    }
    in=pSrc[2u*(i+1)];pSrc[2u*(i+1)]=pSrc[2u*(j+fftLenBy2)];pSrc[2u*(j+fftLenBy2)]=in;
    in=pSrc[2u*(i+1)+1];pSrc[2u*(i+1)+1]=pSrc[2u*(j+fftLenBy2)+1];pSrc[2u*(j+fftLenBy2)+1]=in;
    j=*pBitRevTab;pBitRevTab+=bitRevFactor;
  }
}

/* ----------------------------------------------------------------
 * arm_split_rfft_f32
 * ---------------------------------------------------------------- */
static void split_rfft_f32(float *pSrc, uint32_t fftLen, float *pATable, float *pBTable, float *pDst, uint32_t modifier)
{
  uint32_t i; float outR,outI,*pCoefA,*pCoefB,CoefA1,CoefA2,CoefB1;
  float *pDst1=&pDst[2],*pDst2=&pDst[4u*fftLen-1u];
  float *pSrc1=&pSrc[2],*pSrc2=&pSrc[2u*fftLen-1u];
  pCoefA=&pATable[modifier*2u];pCoefB=&pBTable[modifier*2u];i=fftLen-1u;
  while(i>0u){CoefA1=*pCoefA++;CoefA2=*pCoefA;
    outR=*pSrc1*CoefA1;outI=*pSrc1++*CoefA2;
    outR-=(*pSrc1+*pSrc2)*CoefA2;outI+=*pSrc1++*CoefA1;
    CoefB1=*pCoefB;outI-=*pSrc2--*CoefB1;outI-=*pSrc2*CoefA2;outR+=*pSrc2--*CoefB1;
    *pDst1++=outR;*pDst1++=outI;*pDst2--=-outI;*pDst2--=outR;
    pCoefB+=(modifier*2u);pCoefA+=((modifier*2u)-1u);i--;}
  pDst[2u*fftLen]=pSrc[0]-pSrc[1];pDst[2u*fftLen+1u]=0.0f;
  pDst[0]=pSrc[0]+pSrc[1];pDst[1]=0.0f;
}

/* ----------------------------------------------------------------
 * arm_rfft_f32 embedded
 * ---------------------------------------------------------------- */
static void rfft_f32_emb(const arm_rfft_instance_f32 *S, float *pSrc, float *pDst)
{
  arm_cfft_radix4_instance_f32 *C = (arm_cfft_radix4_instance_f32 *)S->pCfft;
  if(S->ifftFlagR){
    /* RIFFT not used by DCT4 */
    split_rfft_f32(pSrc, S->fftLenBy2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);
    radix4_bf_inv_f32(pDst, C->fftLen, C->pTwiddle, C->twidCoefModifier, C->onebyfftLen);
    if(S->bitReverseFlagR) bitrev_f32_rf(pDst, C->fftLen, C->bitRevFactor, C->pBitRevTable);
  } else {
    radix4_bf_f32(pSrc, C->fftLen, C->pTwiddle, C->twidCoefModifier);
    if(S->bitReverseFlagR) bitrev_f32_rf(pSrc, C->fftLen, C->bitRevFactor, C->pBitRevTable);
    split_rfft_f32(pSrc, S->fftLenBy2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);
  }
}

/* ----------------------------------------------------------------
 * Generate tables for embedded RFFT
 * ---------------------------------------------------------------- */
static arm_rfft_instance_f32 *create_rfft_tables(uint16_t N)
{
    arm_rfft_instance_f32 *S = (arm_rfft_instance_f32 *)malloc(sizeof(arm_rfft_instance_f32));
    if (!S) return 0;
    S->fftLenReal = N;
    S->fftLenBy2 = N / 2;
    S->ifftFlagR = 0;
    S->bitReverseFlagR = 1;

    switch (N) {
        case 4096: S->twidCoefRModifier = 1u; break;
        case 2048: S->twidCoefRModifier = 2u; break;
        case 1024: S->twidCoefRModifier = 4u; break;
        case 512:  S->twidCoefRModifier = 8u; break;
        case 256:  S->twidCoefRModifier = 16u; break;
        case 128:  S->twidCoefRModifier = 32u; break;
        case 64:   S->twidCoefRModifier = 64u; break;
        case 32:   S->twidCoefRModifier = 128u; break;
        default:   S->twidCoefRModifier = 1u;
    }

    { uint32_t i, n = N;
      S->pTwiddleAReal = (float *)malloc(2*n*sizeof(float));
      S->pTwiddleBReal = (float *)malloc(2*n*sizeof(float));
      for (i=0;i<n;i++) {
        double a=2.0*3.14159265358979323846*(double)i/(double)(2u*n);
        S->pTwiddleAReal[2*i]=(float)(0.5*(1.0-sin(a)));S->pTwiddleAReal[2*i+1]=(float)(0.5*(-1.0*cos(a)));
        S->pTwiddleBReal[2*i]=(float)(0.5*(1.0+sin(a)));S->pTwiddleBReal[2*i+1]=(float)(0.5*(1.0*cos(a)));
      }
    }

    arm_cfft_radix4_instance_f32 *C = (arm_cfft_radix4_instance_f32 *)malloc(sizeof(arm_cfft_radix4_instance_f32));
    C->fftLen = N/2; C->ifftFlag = 0; C->bitReverseFlag = 1;
    uint16_t clen = N/2;
    C->pTwiddle = (float *)malloc(clen*2*sizeof(float));
    { uint32_t i; for(i=0;i<clen;i++){
        double a=-2.0*3.14159265358979323846*(double)i/(double)clen;
        C->pTwiddle[2*i]=(float)cos(a);C->pTwiddle[2*i+1]=(float)sin(a);}
    }
    { uint32_t i,bits=0,temp=clen;
      while(temp>1){temp>>=1;bits++;}
      C->pBitRevTable=(uint16_t*)malloc((clen-4)*sizeof(uint16_t));
      for(i=0;i<clen-4;i++){uint32_t rev=0,idx=i+1,b;
        for(b=0;b<bits;b++){rev=(rev<<1)|(idx&1);idx>>=1;}
        C->pBitRevTable[i]=(uint16_t)(rev>>1);}
    }
    C->twidCoefModifier=1;
    C->bitRevFactor=1;
    C->onebyfftLen=1.0f/(float)clen;
    S->pCfft = C;
    return S;
}

static void destroy_rfft_tables(arm_rfft_instance_f32 *S)
{
    if (S) {
        if (S->pTwiddleAReal) free(S->pTwiddleAReal);
        if (S->pTwiddleBReal) free(S->pTwiddleBReal);
        arm_cfft_radix4_instance_f32 *C = (arm_cfft_radix4_instance_f32 *)S->pCfft;
        if (C) {
            if (C->pTwiddle) free(C->pTwiddle);
            if (C->pBitRevTable) free(C->pBitRevTable);
            free(C);
        }
        free(S);
    }
}

/* ----------------------------------------------------------------
 * arm_dct4_f32 - Main DCT4 processing function
 * ---------------------------------------------------------------- */
void arm_dct4_f32(
  const arm_dct4_instance_f32 * S,
  float * pState,
  float * pInlineBuffer)
{
  uint32_t i;
  float *weights = S->pTwiddle;
  float *cosFact = S->pCosFactor;
  float *pS1, *pS2, *pbuff;
  float in;

  /* Pre-processing: r(n) = 2 * u(n) * cos(pi*(2*n+1)/(4*n)) */
  arm_scale_f32(pInlineBuffer, 2.0f, pInlineBuffer, S->N);
  arm_mult_f32(pInlineBuffer, cosFact, pInlineBuffer, S->N);

  /* Step1: Re-order even and odd elements
     pState[i] = pInlineBuffer[2*i]
     pState[N-i-1] = pInlineBuffer[2*i+1] */
  pS1 = pState;
  pS2 = pState + (S->N - 1u);
  pbuff = pInlineBuffer;

  i = S->Nby2;
  do {
    *pS1++ = *pbuff++;
    *pS2-- = *pbuff++;
    i--;
  } while(i > 0u);

  pbuff = pInlineBuffer;
  pS1 = pState;
  i = S->N;
  do {
    *pbuff++ = *pS1++;
    i--;
  } while(i > 0u);

  /* Step2: RFFT of N-point input */
  rfft_f32_emb((const arm_rfft_instance_f32 *)S->pRfft, pInlineBuffer, pState);

  /* Step3: Complex multiply FFT output with weights */
  arm_cmplx_mult_cmplx_f32(pState, weights, pState, S->N);

  /* Post-processing: convert DCT2 to DCT4 */
  pbuff = pInlineBuffer;
  pS1 = pState;

  in = *pS1++ * 0.5f;
  *pbuff++ = in;
  pS1++;

  i = S->N - 1u;
  while(i > 0u) {
    in = *pS1++ - in;
    *pbuff++ = in;
    pS1++;
    i--;
  }

  /* Normalize by sqrt(2/N) */
  pbuff = pInlineBuffer;
  i = S->N;
  do {
    in = *pbuff;
    *pbuff++ = in * S->normalize;
    i--;
  } while(i > 0u);
}