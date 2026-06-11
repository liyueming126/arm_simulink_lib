#include <stdint.h>

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    float *pData;
} arm_matrix_instance_f32;

void arm_mat_init_f32(
  arm_matrix_instance_f32 * S,
  uint16_t nRows,
  uint16_t nColumns,
  float32_t * pData)
{
    S->numRows = nRows;
    S->numCols = nColumns;
    S->pData = pData;
}