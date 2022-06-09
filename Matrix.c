#include "Matrix.h"
#include "List.h"
#include <stdlib.h>
#include <stdio.h>


void printEntryList(FILE*, List);

typedef struct EntryObj *Entry;

typedef struct EntryObj {
    int row;
    int col;
    double val;
} EntryObj;

typedef struct MatrixObj {
    List* matrix;
    int size;
    int nnz;
} MatrixObj;

// --------Constructors and Destructors-----------

// newEntry(), returns a new Entry object
Entry newEntry(int row, int col, double val) {
    Entry E = malloc(sizeof(EntryObj));
    if (E) {
        E->row = row;
        E->col = col;
        E->val = val;
    }
    return E;
}

// freeEntry(), free's Entry object
void freeEntry(Entry *pE) {
    if (pE && *pE) {
        free(*pE);
        *pE = NULL;
    }
    return;
}

// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n) {
    Matrix M = malloc(sizeof(MatrixObj));
    if (M) {
        M->matrix = (List *) calloc(n + 1, sizeof(List));
        for (int i = 0; i < n + 1; i += 1) {
            M->matrix[i] = newList();
        }

        M->size = n;
        M->nnz = 0;
    }
    return M;
}



// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM) {
    if (*pM == NULL) {
        fprintf(stderr, "Matrix Error: Calling freeMatrix() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }


    if (pM && *pM) {
        for (int i = 0; i < size(*pM) + 1; i += 1) {
            List iter = ((*pM)->matrix[i]);
            if (length(iter) > 0) {
                moveFront(iter);
                while (index_pos(iter) >= 0) {
                    // Free all of the entries
                    Entry ent = (Entry) get(iter);
                    // printf("Free'd entry is: (%d, %lf)\n", ent->col, ent->val);
                    if (ent != NULL) {
                        freeEntry(&ent);
                        moveNext(iter);
                        continue;
                    }
                    moveNext(iter);
                }
            }

            // Free the List row
            if (iter != NULL) {
                freeList(&iter);
            }
        }

        free((*pM)->matrix);
        free(*pM);
        *pM = NULL;
    }

    return;
}


// ---------------Access functions------------------

static inline bool matrixEmpty(Matrix M) {
    return (M->size == 0);
}

// size()
// Return the size of square Matrix M.
int size(Matrix M) {
    if (M == NULL) {
        fprintf(stderr, "Matrix Error: Calling size() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    return M->size;
}


// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M) {
    if (M == NULL) {
        fprintf(stderr, "Matrix Error: Calling NNZ() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    return M->nnz;
}


// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B) {
    if (A == NULL) {
        fprintf(stderr, "Matrix Error: Calling equals() on NULL Matrix A reference.\n");
        exit(EXIT_FAILURE);
    }

    if (B == NULL) {
        fprintf(stderr, "Matrix Error: Calling equals() on NULL Matrix B reference.\n");
        exit(EXIT_FAILURE);
    }

    if (size(A) != size(B)) {
        // Not the same size, immediately return false
        return false;
    }

    else if (matrixEmpty(A) && matrixEmpty(B)) {
        // If they are both empty, we can return true
        return true;
    }


    Entry x = NULL;
    Entry y = NULL;
    for (int i = 0; i < size(A) + 1; i += 1) {
        if ((length(A->matrix[i]) == 0) && (length(B->matrix[i]) == 0)) {
            continue;
        }

        if ((length(A->matrix[i]) == 0) || (length(B->matrix[i]) == 0)) {
            return false;
        }

        if (length(A->matrix[i]) != length(B->matrix[i])) {
            return false;
        }

        if ((length(A->matrix[i]) != 0) && (length(B->matrix[i]) != 0)) {
            moveFront(A->matrix[i]);
            moveFront(B->matrix[i]);
            while (index_pos(A->matrix[i]) >= 0) {
                x = (Entry) get(A->matrix[i]);
                y = (Entry) get(B->matrix[i]);
                if ((x->val != y->val) || (x->col != y->col) || (x->row != y->row)) {
                    return false;
                }
                moveNext(A->matrix[i]);
                moveNext(B->matrix[i]);
            }
        }
    }

    return true;
}


// --------------Manipulation procedures--------------

// makeZero()
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M) {
    if (M == NULL) {
        fprintf(stderr, "Matrix Error: Calling makeZero() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    // Iterate through the lists, and clear them
    for (int i = 0; i < size(M) + 1; i += 1) {
        List iter = M->matrix[i];
        // Iterate through the list
        if (length(iter) > 0) {
            moveFront(iter);
            while (index_pos(iter) >= 0) {
                Entry ent = (Entry) get(iter);
                if (ent != NULL) {
                    freeEntry(&ent);
                    moveNext(iter);
                    continue;
                }
                moveNext(iter);
            }
        }
        if (iter != NULL) {
            clear(M->matrix[i]);
        }
    }

    M->nnz = 0;
    return;
}


// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x) {
    if (M == NULL) {
        fprintf(stderr, "Matrix Error: Calling changeEntry() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    if (((1 <= i) && (i <= size(M))) != true) {
        fprintf(stderr, "Matrix Error: changeEntry() Violated precondition 1 < i < size(M).\n");
        exit(EXIT_FAILURE);
    }

    if (((1 <= j) && (j <= size(M))) != true) {
        fprintf(stderr, "Matrix Error: changeEntry() Violated precondition 1 < j < size(M).\n");
        exit(EXIT_FAILURE);
    }

    if (x == 0.0) {
        // Can't include 0.0 in our sparse matrix repr
        List Row = M->matrix[i];
        if (length(Row) > 0) {
            moveFront(Row);
            while (index_pos(Row) >= 0) {
                Entry ent = (Entry) get(Row);
                if (ent->col == j) {
                    M->nnz -= 1;
                    freeEntry(&ent);
                    delete(Row);
                    break;
                }
                moveNext(Row);
            }
        }
        return;
    }

    List mat = M->matrix[i];
    if (length(mat) > 0) {
        moveFront(mat);
        // Loop to see if something is already in the slot, if so, free it
        while (index_pos(mat) >= 0) {
            Entry ent = (Entry) get(mat);
            if (ent->col == j) {
                // There is already something here, we need to free Entry,
                // delete Node
                freeEntry(&ent);
                delete(mat);
                break;
            }
            moveNext(mat);
        }

        if (length(mat) > 0) {
            // Insert into correct place, sorted by col order
            moveFront(mat);
            for (int k = 1; k < length(mat) + 1; k += 1) {
                Entry ent = (Entry) get(mat);
                Entry insert = newEntry(i, j, x);
                if (j < ent->col) {
                    insertBefore(mat, insert);
                    break;
                }
                moveNext(mat);
                if (index_pos(mat) < 0) {
                    append(mat, insert);
                    break;
                }
                // If we can't insert Entry anywhere, we must free it
                freeEntry(&insert);
            }
        }

        else {
            // Our deletion removed the only element in the List, append
            Entry ent = newEntry(i, j, x);
            append(mat, ent);
        }
    }

    else {
        // The row is empty, just append to it
        Entry ent = newEntry(i, j, x);
        append(mat, ent);
    }

    M->nnz += 1;

    return;
}


// ------------Matrix Arithmetic operations-----------------
// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A) {
    if (A == NULL) {
        fprintf(stderr, "Matrix Error: Calling copy() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    Matrix B = newMatrix(size(A));

    for (int i = 0; i < size(A) + 1; i += 1) {
        List Row = A->matrix[i];
        if (length(Row) > 0) {
            moveFront(Row);
            while (index_pos(Row) >= 0) {
                Entry ent = (Entry) get(Row);
                int j = ent->col;
                changeEntry(B, i, j, ent->val);
                moveNext(Row);
            }
        }
    }

    B->size = A->size;
    B->nnz = A->nnz;

    return B;
}


// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A) {
    if (A == NULL) {
        fprintf(stderr, "Matrix Error: Calling transpose() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    Matrix B = newMatrix(size(A));

    for (int i = 0; i < size(A) + 1; i += 1) {
        List Row = A->matrix[i];
        if (length(Row) > 0) {
            moveFront(Row);
            while (index_pos(Row) >= 0) {
                Entry ent = (Entry) get(Row);
                int j = ent->col;
                changeEntry(B, j, i, ent->val);
                moveNext(Row);
            }
        }
    }

    B->size = A->size;
    B->nnz = A->nnz;

    return B;
}


// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A) {
    if (A == NULL) {
        fprintf(stderr, "Matrix Error: Calling transpose() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    Matrix M = newMatrix(size(A));

    for (int i = 1; i < size(A) + 1; i += 1) {
        // Iterate through rows
        List col = A->matrix[i];
        if (length(col) > 0) {
            moveFront(col);
            while (index_pos(col) >= 0) {
                Entry ent = (Entry) get(col);
                int j = ent->col;
                double input_val = x * ent->val;
                changeEntry(M, i, j, input_val);
                moveNext(col);
            }
        }
    }

    M->size = A->size;
    M->nnz= A->nnz;

    return M;
}

// Private helper function, shallow copy of Entry List
List copyEntryList(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: Calling copyEntryList() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        return L;
    }

    List R = newList();

    // Iterate through input List L
    moveFront(L);
    while (index_pos(L) >= 0) {
        // printEntryList(stdout, L);
        Entry ent = (Entry) get(L);
        Entry val = newEntry(ent->row, ent->col, ent->val);
        append(R, val);
        moveNext(L);
    }

    return R;

}




List SumDiffList(List P, List Q, bool sum, int* nnz_val) {
    if (P == NULL) {
        fprintf(stderr, "List Error: Calling SumDiffList() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (Q == NULL) {
        fprintf(stderr, "List Error: Calling SumDiffList() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }


    if ((length(P) == 0) && length(Q) == 0) {
        // If they're both 0, just return empty list
        return newList();
    }

    else if ((length(P) > 0) && (length(Q) == 0)) {
        // P is only list with items being added, return it
        List R = copyEntryList(P);
        *nnz_val += length(P);
        return R;
    }

    else if ((length(Q) > 0) && (length(P) == 0)) {
        // Q is only list with items being added, return it
        List R = copyEntryList(Q);
        *nnz_val += length(Q);
        return R;
    }

    List R = newList();

    moveFront(P);
    moveFront(Q);
    // while they both aren't off the list
    while ((index_pos(P) >= 0) && ((index_pos(Q)) >= 0)) {
        Entry ent_P = (Entry) get(P);
        Entry ent_Q = (Entry) get(Q);
        if (ent_P->col == ent_Q->col) {
            if (sum) {
                Entry add = newEntry(ent_P->row, ent_P->col, ent_P->val + ent_Q->val);
                if (add->val != 0.0) {
                    append(R, add);
                    *nnz_val += 1;
                }
                else {
                    freeEntry(&add);
                }
            }
            else {
                Entry diff = newEntry(ent_P->row, ent_P->col, ent_P->val - ent_Q->val);
                if (diff->val != 0.0) {
                    append(R, diff);
                    *nnz_val += 1;
                }
                else {
                    freeEntry(&diff);
                }
            }
        }
        else if (ent_P->col < ent_Q->col) {
            // Q is ahead
            Entry add = newEntry(ent_P->row, ent_P->col, ent_P->val);
            append(R, add);
            *nnz_val += 1;
            moveNext(P);
            continue;
        }
        else {
            // P is ahead
            Entry add = newEntry(ent_Q->row, ent_Q->col, ent_Q->val);
            if (sum == false) {
                double Val = add->val;
                add->val = -1.0 * Val;
            }
            append(R, add);
            *nnz_val += 1;
            moveNext(Q);
            continue;
        }

        moveNext(P);
        moveNext(Q);

    }

    while (index_pos(P) >= 0) {
        // P still has stuff
        Entry ent = (Entry) get(P);
        Entry add = newEntry(ent->row, ent->col, ent->val);
        append(R, add);
        *nnz_val += 1;
        moveNext(P);
    }

    while (index_pos(Q) >= 0) {
        // Q still has stuff
        Entry ent = (Entry) get(Q);
        Entry add = newEntry(ent->row, ent->col, ent->val);
        if (sum == false) {
            double Val = add->val;
            add->val = -1.0 * Val;
        }
        append(R, add);
        *nnz_val += 1;
        moveNext(Q);
    }

    return R;
}

// Shallow helper function, find xP, where P is a vector (List), and x is a scalar
List vectorScalarMult(double val, List P) {
    List R = newList();
    moveFront(P);
    while (index_pos(P) >= 0) {
        Entry ent = (Entry) get(P);
        Entry mult = newEntry(ent->row, ent->col, ent->val * val);
        append(R, mult);
        moveNext(P);
    }

    return R;
}

// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B) {
    if (A == NULL) {
        fprintf(stderr, "Matrix Error: Calling sum() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    if (B == NULL) {
        fprintf(stderr, "Matrix Error: Calling sum() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    if (size(A) != size(B)) {
        fprintf(stderr, "Matrix Error: Size of A and B don't match.\n");
        exit(EXIT_FAILURE);
    }

    if ((equals(A, B)) == true) {
        return scalarMult(2.0, A);
    }

    Matrix M = newMatrix(size(A));

    int non_z = 0;
    for (int i = 1; i < size(A) + 1; i += 1) {
        List col_A = A->matrix[i];
        List col_B = B->matrix[i];
        if (length(col_A) > 0 || (length(col_B) > 0)) {
            freeList(&(M->matrix[i]));
            M->matrix[i] = SumDiffList(col_A, col_B, true, &non_z);
        }
    }
    M->nnz = non_z;

    return M;
}


// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B) {
    if (A == NULL) {
        fprintf(stderr, "Matrix Error: Calling diff() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    if (B == NULL) {
        fprintf(stderr, "Matrix Error: Calling diff() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    if (size(A) != size(B)) {
        fprintf(stderr, "Matrix Error: Size of A and B don't match.\n");
        exit(EXIT_FAILURE);
    }

    Matrix M = newMatrix(size(A));

    int non_z = 0;

    if (equals(A, B) != true) {
        for (int i = 1; i < size(A) + 1; i += 1) {
            List col_A = A->matrix[i];
            List col_B = B->matrix[i];
            if (length(col_A) > 0 && (length(col_B) > 0)) {
                freeList(&(M->matrix[i]));
                M->matrix[i] = SumDiffList(col_A, col_B, false, &non_z);
            }

            else if ((length(col_B) > 0) && (length(col_A) == 0)) {
                // 0 - a matrix
                freeList(&(M->matrix[i]));
                M->matrix[i] = vectorScalarMult(-1.0, col_B);
                M->nnz += length(col_B);
            }
            else if ((length(col_A) > 0) && (length(col_B) == 0)) {
                freeList(&(M->matrix[i]));
                M->matrix[i] = copyEntryList(col_A);
                M->nnz += length(col_A);
            }
        }
    }

    M->nnz += non_z;

    return M;
}


double vectorDot(List P, List Q) {
    if (P == NULL) {
        fprintf(stderr, "List Error: Calling vectorDot() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (Q == NULL) {
        fprintf(stderr, "List Error: Calling vectorDot() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    double dot = 0;

    if ((length(P) == 0) || length(Q) == 0) {
        // If they're one of our vectors is 0, we return 0
        return 0;
    }

    if ((length(P) > 0) && length(Q) > 0) {
        moveFront(P);
        moveFront(Q);
        while ((index_pos(P) >= 0) && index_pos(Q) >= 0) {
            double val_P = 0.0;
            double val_Q = 0.0;

            Entry ent_P = (Entry) get(P);
            Entry ent_Q = (Entry) get(Q);

            if (ent_P->col == ent_Q->col) {
                val_P = ent_P->val;
                val_Q = ent_Q->val;
            }

            else {
                if (ent_Q->col < ent_P->col) {
                    moveNext(Q);
                    continue;
                }
                else {
                    moveNext(P);
                    continue;
                }
            }

            dot += val_P * val_Q;

            moveNext(P);
            moveNext(Q);
        }
    }

    return dot;
}

// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B) {
    if (A == NULL) {
        fprintf(stderr, "Matrix Error: Calling product() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    if (B == NULL) {
        fprintf(stderr, "Matrix Error: Calling product() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    if (size(A) != size(B)) {
        fprintf(stderr, "Matrix Error: Size of A and B don't match.\n");
        exit(EXIT_FAILURE);
    }

    Matrix P = newMatrix(size(A));

    Matrix C = transpose(B);

    for (int i = 1; i < size(A) + 1; i += 1) {
        List A_mat = A->matrix[i];
        if ((length(A_mat) == 0)) {
            continue;
        }

        for (int j = 1; j < size(C) + 1; j += 1) {
            double dot_prod = vectorDot(A_mat, C->matrix[j]);
            changeEntry(P, i, j, dot_prod);
        }
    }

    freeMatrix(&C);

    return P;
}

// Shallow function to print List of type Entry
void printEntryList(FILE *out, List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: Calling printEntryList() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    // Assuming the list length iength > 0
    if (length(L) == 0) {
        fprintf(stderr, "Trying to print List of length 0.\n");
        exit(EXIT_FAILURE);
    }

    moveFront(L);
    while (index_pos(L) >= 0) {
        Entry Col = (Entry) get(L);
        int col = Col->col;
        double val = Col->val;
        fprintf(out, " (%d, %.1f)", col, val);
        moveNext(L);
    }

    return;
}


// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero row is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M) {
    if (M == NULL) {
        fprintf(stderr, "Matrix Error: Calling printMatrix() on NULL Matrix reference.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size(M) + 1; i += 1) {
        List Row = M->matrix[i];
        if (length(Row) > 0) {
            fprintf(out, "%d:", i);
            printEntryList(out, Row);
            fprintf(out, "\n");
        }
    }

    return;
}

