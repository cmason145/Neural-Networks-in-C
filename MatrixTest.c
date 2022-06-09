#include "Matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

int main(int argv, char **argc) {
    // Lets make a 5x5 matrix
    Matrix A = newMatrix(3);

    Matrix M = newMatrix(5);
    Matrix L = newMatrix(5);

    // Another 10x10 matrix
    Matrix N = newMatrix(10);


    // -----Size tests-----
    int size_score = 0;

    if (size(M) != 5) {
        fprintf(stderr, "Matrix size of M is not 5!\n");
        fprintf(stderr, "Instead, it is: %d.\n", size(M));
    }

    else {
        fprintf(stdout, "Correct size for matrix M!\n");
        size_score += 5;
    }


    if (size(N) != 10) {
        fprintf(stderr, "Matrix size of M is not 10!\n");
        fprintf(stderr, "Instead, it is: %d.\n", size(M));
    }

    else {
        fprintf(stdout, "Correct size for matrix N!\n");
        size_score += 5;

    }

    fprintf(stdout, "Score for size tests: %d out of 10 points.\n", size_score);
    fprintf(stdout, "\n");

    // -------NNZ Tests--------
    int nnz_score = 0;
    if (NNZ(M) != 0) {
        fprintf(stderr, "Matrix M has non-zero elements!\n");
        fprintf(stderr, "Number of non-zero elements is: %d.\n", NNZ(M));
    }

    else {
        fprintf(stdout, "Correct number of non-zero elements for matrix M!\n");
        nnz_score += 5;
    }


    if (NNZ(N) != 0) {
        fprintf(stderr, "Matrix N has non-zero elements!\n");
        fprintf(stderr, "Number of non-zero elements is: %d.\n", NNZ(N));
    }

    else {
        fprintf(stdout, "Correct number of non-zero elements for matrix N!\n");
        nnz_score += 5;
    }

    fprintf(stdout, "Score for NNZ tests: %d out of 10 points.\n", nnz_score);
    fprintf(stdout, "\n");

    // ---------Equality tests--------
    int equality_score = 0;
    if (equals(M, N) == true) {
        fprintf(stderr, "M and N are equal, when they shouldn't be!\n");
    }

    else {
        fprintf(stdout, "M and N are not equal, correct equality.\n");
        equality_score += 5;
    }

    if (equals(M, L) == false) {
        fprintf(stderr, "M and L aren't equal when they should be!\n");
    }

    else {
        fprintf(stdout, "M and L are equal, correct equality.\n");
        equality_score += 5;
    }

    fprintf(stdout, "Score for equality tests: %d out of 10", equality_score);
    fprintf(stdout, "\n");

    // Before testing manipulation procedures, test changeEntry and printMatrix

    printf("Changing row 1 col 1 to 2.0....\n");
    changeEntry(M, 1, 1, 2.0);
    printf("We have changed the first index.\n");
    printMatrix(stdout, M);
    printf("End of test for inserting one item.\n");

    // Add a bunch of items in the first row to 2.0
    for (int i = 1; i < size(M) + 1; i += 1) {
        changeEntry(M, 1, i, 3.0);
    }

    // Add a bunch more to the second row
    for (int j = 1; j < size(M) + 1; j += 1) {
        changeEntry(M, 2, j, 4.0);
    }

    printf("We have changed all of the entries.\n");

    printMatrix(stdout, M);

    printf("Changing entry at row2 col3 to 7.0...\n");

    changeEntry(M, 2, 3, 7.0);

    printf("Changed entry at row2 col3!\n");

    printMatrix(stdout, M);

    Matrix C = copy(M);

    bool check_C = equals(C, M);

    if (check_C) {
        printf("C and M are equal as they should be, pass!\n");
    }

    else {
        printf("Equals is wrong, C and M should be equal!\n");
    }

    Matrix T = transpose(M);

    printf("Transpose of matrix M is: \n");
    printMatrix(stdout, T);

    printf("Lets try and make a massive fucking matrix.\n");
    Matrix P = newMatrix(1000);

    printf("\n");
    printf("Now we're testing our scalar multiply function");
    Matrix S = scalarMult(3.0, M);

    printf("Printing this scalar multiplied matrix....\n");
    printMatrix(stdout, S);
    printf("\n");

    printf("Time for matrix addition amazing.\n");

    Matrix F = sum(S, M);

    printf("Addition of S and M is: \n");
    printMatrix(stdout, F);

    printf("Time for matrix difference tests, beautiful :)\n");

    Matrix E = diff(S, M);

    printf("Difference of S and M is: \n");
    printMatrix(stdout, E);

    Matrix D = diff(M, S);

    printf("Difference of M and S is: \n");
    printMatrix(stdout, D);

    printf("Exiting....\n");

    printf("\n");
    printf("Dot product tests, here we go boys.\n");
    printf("First, to initialize a bunch more matrices.\n");

    Matrix X = newMatrix(3);
    Matrix W = newMatrix(3);

    changeEntry(X, 1, 1, 1.0);
    changeEntry(X, 1, 2, 2.0);
    changeEntry(X, 1, 3, 3.0);
    changeEntry(X, 2, 1, 4.0);
    changeEntry(X, 2, 2, 5.0);
    changeEntry(X, 2, 3, 6.0);
    changeEntry(X, 3, 1, 7.0);
    changeEntry(X, 3, 2, 8.0);
    changeEntry(X, 3, 3, 9.0);

    changeEntry(W, 1, 1, 1.0);
    changeEntry(W, 1, 3, 1.0);
    changeEntry(W, 3, 1, 1.0);
    changeEntry(W, 3, 2, 1.0);
    changeEntry(W, 3, 3, 1.0);

    printf("Print out the matrices: \n");
    printf("\n");

    printf("Matrix X: \n");

    printMatrix(stdout, X);

    printf("Matrix W: \n");
    printMatrix(stdout, W);

    printf("Transpose of Matrix W: \n");
    Matrix I = transpose(W);
    printMatrix(stdout, I);

    printf("Matrix XW is: \n");

    Matrix Z = product(X, W);
    printMatrix(stdout, Z);
    printf("\n");

    freeMatrix(&A);

    A = newMatrix(100);

    changeEntry(A, 1, 1, -4);
    changeEntry(A, 1, 2, -2);
    changeEntry(A, 1, 3, 0);
    changeEntry(A, 2, 5, 4);
    changeEntry(A, 2, 1, -2);
    changeEntry(A, 3, 1, 2);
    changeEntry(A, 2, 2, -2);
    changeEntry(A, 3, 3, 0);

    printf("Matrix A is: \n");
    printMatrix(stdout, A);
    printf("\n");

    Matrix K = diff(A, A);
    if (NNZ(K) != 0 || NNZ(A) != 6) {
        printf("NNZ of K is 0, or NNZ of A is NOT 6.\n");
        printf("NNZ(K) = %d, and NNZ(A) = %d.\n", NNZ(K), NNZ(A));
    }



    printf("Matrix B tests.\n");

    Matrix B = newMatrix(100);

    changeEntry(B, 1, 1, -4);
    changeEntry(B, 1, 2, 0);
    changeEntry(B, 2, 1, 0);
    changeEntry(B, 2, 2, -2);
    changeEntry(B, 2, 4, 2);
    changeEntry(B, 3, 1, 2);
    changeEntry(B, 3, 2, 2);
    changeEntry(B, 7, 8, 5);

    printf("Matrix B is: \n");
    printMatrix(stdout, B);
    printf("\n");

    Matrix J = diff(A, B);
    printf("Matrix A - B is: \n");
    printMatrix(stdout, J);
    printf("\n");

    if (NNZ(J) != 6) {
        printf("NNZ of J is not 6, instead it is: %d.\n", NNZ(J));
    }

    freeMatrix(&A);
    freeMatrix(&B);

    A = newMatrix(10);
    B = newMatrix(10);


    changeEntry(A, 1, 1, 4);
    changeEntry(A, 1, 2, 2);
    changeEntry(A, 1, 3, 0);
    changeEntry(A, 2, 1, 2);
    changeEntry(A, 3, 1, 0);
    changeEntry(A, 2, 2, 2);
    changeEntry(A, 3, 3, 0);

    printf("Matrix A is: \n");
    printMatrix(stdout, A);
    printf("\n");

    Matrix Q = sum(A, A);

    printf("Matrix Q, A + A is: \n");
    printMatrix(stdout, Q);
    printf("\n");

    if (NNZ(Q) != 4 || NNZ(A) != 4) {
        printf("NNZ of Q is not 4, or NNZ of A is not 4.\n");
    }

    changeEntry(B, 1, 1, -4);
    changeEntry(B, 1, 2, 0);
    changeEntry(B, 2, 1, 0);
    changeEntry(B, 2, 2, -2);
    changeEntry(B, 2, 4, 2);
    changeEntry(B, 3, 1, 0);
    changeEntry(B, 3, 2, 2);
    changeEntry(B, 7, 8, 5);

    printf("Matrix B is: \n");
    printMatrix(stdout, B);
    printf("\n");

    Matrix O = sum(A, B);

    printf("Matrix O, A + B is: \n");
    printMatrix(stdout, O);
    printf("\n");

    if (NNZ(O) != 5) {
        printf("NNZ of O is not 5. Instead, it is: %d\n", NNZ(O));
    }

    freeMatrix(&A);
    freeMatrix(&Q);
    freeMatrix(&O);
    freeMatrix(&J);
    freeMatrix(&K);
    freeMatrix(&C);
    freeMatrix(&S);
    freeMatrix(&F);
    freeMatrix(&E);
    freeMatrix(&D);
    freeMatrix(&T);
    freeMatrix(&I);
    freeMatrix(&M);
    freeMatrix(&P);
    freeMatrix(&N);
    freeMatrix(&L);
    freeMatrix(&X);
    freeMatrix(&W);
    freeMatrix(&Z);
    return 0;
}
