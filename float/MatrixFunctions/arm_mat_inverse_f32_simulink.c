#include <stdint.h>
#include <math.h>

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    float *pData;
} arm_matrix_instance_f32;

typedef enum {
    ARM_MATH_SUCCESS = 0,
    ARM_MATH_SIZE_MISMATCH = -1,
    ARM_MATH_SINGULAR = -2
} arm_status;

arm_status arm_mat_inverse_f32(
  const arm_matrix_instance_f32 * pSrc,
  arm_matrix_instance_f32 * pDst)
{
    float *pIn = pSrc->pData;
    float *pOut = pDst->pData;
    float *pInT1, *pInT2;
    float *pOutT1, *pOutT2;
    float *pPivotRowIn, *pPRT_in, *pPivotRowDst, *pPRT_pDst;
    uint32_t numRows = pSrc->numRows;
    uint32_t numCols = pSrc->numCols;
    float Xchg, in = 0.0f;
    uint32_t i, rowCnt, flag = 0u, j, loopCnt, k, l;
    arm_status status;

    if((pSrc->numRows != pSrc->numCols) || (pDst->numRows != pDst->numCols)
       || (pSrc->numRows != pDst->numRows))
    {
        status = ARM_MATH_SIZE_MISMATCH;
    }
    else
    {
        pOutT1 = pOut;
        rowCnt = numRows;
        while(rowCnt > 0u)
        {
            j = numRows - rowCnt;
            while(j > 0u) { *pOutT1++ = 0.0f; j--; }
            *pOutT1++ = 1.0f;
            j = rowCnt - 1u;
            while(j > 0u) { *pOutT1++ = 0.0f; j--; }
            rowCnt--;
        }

        loopCnt = numCols;
        l = 0u;
        while(loopCnt > 0u)
        {
            pInT1 = pIn + (l * numCols);
            pOutT1 = pOut + (l * numCols);
            in = *pInT1;
            flag = 0u;
            k = 1u;

            if(*pInT1 == 0.0f)
            {
                for(i = (l + 1u); i < numRows; i++)
                {
                    pInT2 = pInT1 + (numCols * l);
                    pOutT2 = pOutT1 + (numCols * k);
                    if(*pInT2 != 0.0f)
                    {
                        for(j = 0u; j < (numCols - l); j++)
                        {
                            Xchg = *pInT2; *pInT2++ = *pInT1; *pInT1++ = Xchg;
                        }
                        for(j = 0u; j < numCols; j++)
                        {
                            Xchg = *pOutT2; *pOutT2++ = *pOutT1; *pOutT1++ = Xchg;
                        }
                        flag = 1u;
                        break;
                    }
                    k++;
                }
            }
            else
            {
                flag = 1u;
            }

            if((flag != 1u) && (in == 0.0f))
            {
                return ARM_MATH_SINGULAR;
            }

            pPivotRowIn = pIn + (l * numCols);
            pPivotRowDst = pOut + (l * numCols);
            pInT1 = pPivotRowIn;
            pOutT1 = pPivotRowDst;
            in = *(pIn + (l * numCols));

            for(j = 0u; j < (numCols - l); j++)
            {
                *pInT1 = *pInT1 / in; pInT1++;
            }
            for(j = 0u; j < numCols; j++)
            {
                *pOutT1 = *pOutT1 / in; pOutT1++;
            }

            pInT1 = pIn;
            pOutT1 = pOut;
            for(i = 0u; i < numRows; i++)
            {
                if(i == l)
                {
                    pInT1 += numCols - l;
                    pOutT1 += numCols;
                }
                else
                {
                    in = *pInT1;
                    pPRT_in = pPivotRowIn;
                    pPRT_pDst = pPivotRowDst;
                    for(j = 0u; j < (numCols - l); j++)
                    {
                        *pInT1 = *pInT1 - (in * *pPRT_in++); pInT1++;
                    }
                    for(j = 0u; j < numCols; j++)
                    {
                        *pOutT1 = *pOutT1 - (in * *pPRT_pDst++); pOutT1++;
                    }
                }
                pInT1 = pInT1 + l;
            }
            pIn++;
            loopCnt--;
            l++;
        }

        status = ARM_MATH_SUCCESS;
        if((flag != 1u) && (in == 0.0f))
        {
            pIn = pSrc->pData;
            for(i = 0; i < numRows * numCols; i++)
            {
                if(pIn[i] != 0.0f) break;
            }
            if(i == numRows * numCols)
                status = ARM_MATH_SINGULAR;
        }
    }
    return status;
}