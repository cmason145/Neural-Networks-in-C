#include "Matrix.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct row_list row_list;
typedef struct value_list value_list;

// Node to represent row - list
struct row_list
{
    uint32_t row_number;
    row_list *link_down;
    value_list *link_right;
};
 
// Node to represent triples
struct value_list
{
    uint32_t column_index;
    double value;
    value_list *next;
};

// Main matrix struct
struct Matrix {
    row_list** matrix;
    uint32_t rows;
    uint32_t cols;
    uint32_t nnz;
};

// Creates new Matrix in empty state
Matrix* matrix_create(uint32_t rows, uint32_t cols) {
    Matrix* M = (Matrix*) malloc(sizeof(Matrix));
    if (M) {
        M->rows = rows;
        M->cols = cols;
        M->nnz = 0;
        M->matrix = (row_list**) calloc(rows + 1, sizeof(row_list*));
        

    }
}


