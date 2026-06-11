/* Simplified implementation of ARM FIR sparse filter for Simulink.
 * Based on CMSIS arm_fir_sparse_f32.c (Cortex-M0 version).
 * Inline circular buffer operations used instead of arm_circularRead/Write.
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;

typedef struct {
    uint16_t numTaps;
    int32_t stateIndex;
    float32_t *pState;
    float32_t *pCoeffs;
    uint16_t maxDelay;
    int32_t *pTapDelay;
} arm_fir_sparse_instance_f32;

/* Inline circular write: copy blockSize samples from pSrc to circular buffer py of size delaySize */
static void circular_write_f32(float32_t *py, uint32_t delaySize, int32_t *stateIndex,
                                int32_t *pSrc, uint32_t blockSize)
{
    uint32_t i;
    int32_t idx = *stateIndex;
    for (i = 0; i < blockSize; i++) {
        py[idx] = pSrc[i];
        idx++;
        if (idx >= (int32_t)delaySize) idx = 0;
    }
    *stateIndex = idx;
}

/* Inline circular read: copy blockSize samples from circular buffer py to pb */
static void circular_read_f32(float32_t *py, uint32_t delaySize, int32_t *readIndex,
                               float32_t *pb, uint32_t blockSize)
{
    uint32_t i;
    int32_t idx = *readIndex;
    for (i = 0; i < blockSize; i++) {
        pb[i] = py[idx];
        idx++;
        if (idx >= (int32_t)delaySize) idx = 0;
    }
    *readIndex = idx;
}

void arm_fir_sparse_f32(
    arm_fir_sparse_instance_f32 * S,
    float32_t * pSrc,
    float32_t * pDst,
    float32_t * pScratchIn,
    uint32_t blockSize)
{
    float32_t *pState = S->pState;
    float32_t *pCoeffs = S->pCoeffs;
    float32_t *px;
    float32_t *py = pState;
    float32_t *pb = pScratchIn;
    float32_t *pOut;
    int32_t *pTapDelay = S->pTapDelay;
    uint32_t delaySize = S->maxDelay + blockSize;
    uint16_t numTaps = S->numTaps;
    int32_t readIndex;
    uint32_t tapCnt, blkCnt;
    float32_t coeff;
    uint32_t i;

    /* Copy input samples into circular state buffer */
    circular_write_f32(py, delaySize, &S->stateIndex, (int32_t *)pSrc, blockSize);

    /* Calculate read index */
    readIndex = ((int32_t)S->stateIndex - (int32_t)blockSize) - *pTapDelay++;
    if (readIndex < 0) readIndex += (int32_t)delaySize;

    py = pState;
    circular_read_f32(py, delaySize, &readIndex, pb, blockSize);
    px = pb;
    pOut = pDst;

    coeff = *pCoeffs++;

    /* First tap */
    for (i = 0; i < blockSize; i++) {
        pOut[i] = px[i] * coeff;
    }

    /* Remaining taps */
    tapCnt = (uint32_t)numTaps - 2u;
    while (tapCnt > 0u)
    {
        coeff = *pCoeffs++;
        readIndex = ((int32_t)S->stateIndex - (int32_t)blockSize) - *pTapDelay++;
        if (readIndex < 0) readIndex += (int32_t)delaySize;

        py = pState;
        circular_read_f32(py, delaySize, &readIndex, pb, blockSize);
        px = pb;
        pOut = pDst;

        for (i = 0; i < blockSize; i++) {
            pOut[i] += px[i] * coeff;
        }

        tapCnt--;
    }

    /* Last tap */
    coeff = *pCoeffs;
    readIndex = ((int32_t)S->stateIndex - (int32_t)blockSize) - *pTapDelay;
    if (readIndex < 0) readIndex += (int32_t)delaySize;

    py = pState;
    circular_read_f32(py, delaySize, &readIndex, pb, blockSize);
    px = pb;
    pOut = pDst;

    for (i = 0; i < blockSize; i++) {
        pOut[i] += px[i] * coeff;
    }
}