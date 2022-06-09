#pragma once

#include <stdio.h>
#include <stdint.h>
#include <math.h>

typedef struct Matrix Matrix;

Matirix* matrix_create(uint32_t rows, uint32_t cols);

void matrix_free(Matrix** M);

uint32_t row_count(Matrix* M);

uint32_t size(Matrix* M);

// Returns number of non-zero elements in matrix
uint32_t nnz(Matrix* M);



