#include <stdio.h>
#include "List.h"
#include "Matrix.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char **argv) {
    if ((argc < 3) || (argc < 0)) {
        fprintf(stderr, "Error: Too many input files, or too few. Exiting...\n");
        exit(EXIT_FAILURE);
    }

    FILE *input = stdin;
    FILE *output = stdout;

    if ((input = fopen(argv[1], "r"))== NULL) {
        fprintf(stderr, "File Error: Failed to open input file. Exiting...\n");
        exit(EXIT_FAILURE);
    }

    if ((output = fopen(argv[2], "w")) == NULL) {
        fprintf(stderr, "File Error: Failed to open output file. Exiting...\n");

        fclose(input);
        exit(EXIT_FAILURE);
    }

    // Get n, a, and b
    int n, a, b;
    fscanf(input, "%d" "%d" "%d" "\n", &n, &a, &b);

    // char buff[100];
    // fgets(buff, 100, input);

    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);

    // printf("We have gotten to the initializing the matrices.\n");

    int i, j;
    double val;
    for (int k = 0; k < a; k += 1) {
        fscanf(input, "%d" "%d" "%lf" "\n", &i, &j, &val);
        // printf("Value of i, j, and val are: %d %d %lf\n", i, j, val);
        changeEntry(A, i, j, val);
    }

    i = 0, j = 0;
    val = 0.0;


    for (int k = 0; k < b; k += 1) {
        fscanf(input, "%d" "%d" "%lf" "\n", &i, &j, &val);
        // printf("Value of i, j, and val are: %d %d %lf\n", i, j, val);
        changeEntry(B, i, j, val);
    }


    fprintf(output, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(output, A);
    fprintf(output, "\n");

    fprintf(output, "B has %d non-zero entries:\n", NNZ(B));
    printMatrix(output, B);
    fprintf(output, "\n");

    fprintf(output, "(1.5)*A =\n");
    Matrix C = scalarMult(1.5, A);
    printMatrix(output, C);
    fprintf(output, "\n");

    fprintf(output, "A+B =\n");
    Matrix D = sum(A, B);
    printMatrix(output, D);
    fprintf(output, "\n");

    fprintf(output, "A+A =\n");
    Matrix E = sum(A, A);
    printMatrix(output, E);
    fprintf(output, "\n");

    fprintf(output, "B-A =\n");
    Matrix F = diff(B, A);
    printMatrix(output, F);
    fprintf(output, "\n");

    fprintf(output, "A-A =\n");
    Matrix G = diff(A, A);
    printMatrix(output, G);
    fprintf(output, "\n");

    fprintf(output, "Transpose(A) =\n");
    Matrix H = transpose(A);
    printMatrix(output, H);
    fprintf(output, "\n");


    fprintf(output, "A*B =\n");
    Matrix I = product(A, B);
    printMatrix(output, I);
    fprintf(output, "\n");

    fprintf(output, "B*B =\n");
    Matrix J = product(B, B);
    printMatrix(output, J);
    fprintf(output, "\n");

    fclose(input);
    fclose(output);

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    freeMatrix(&J);

    return 0;
}
