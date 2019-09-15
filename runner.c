#include <stdio.h>
#include "matrix.h"

int main()
{
    int t, m, n, z;
    matrix *A, *xp, *xn, *b;
#ifdef FILE_IN
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
#ifndef FILE_IN
    printf("Enter number of test cases: ");
#endif
    scanf("%d", &t);
    for (z = 0; z < t; z++)
    {
#ifndef FILE_IN
        printf("Enter m and n: ");
#endif
        scanf("%d %d", &m, &n);
        printf("Test %d\n", z + 1);
        A = newMatrix(m, n);
#ifndef FILE_IN
        printf("Enter A: \n");
#endif
        readMatrix(A, stdin);
        b = newMatrix(m, 1);
#ifndef FILE_IN
        printf("Enter b: \n");
#endif
        readMatrix(b, stdin);
        rref(A, b);
#ifdef DEBUG_MAIN
        printMatrix(A);
        printMatrix(b);
#endif
        if (!solutionExists(A, b))
        {
            printf("No-solution\n");
            continue;
        }
#ifdef DEBUG_MAIN
        printMatrix(A);
        printMatrix(b);
#endif
        xp = particularSolution(A, b);
        xn = nullspaceSolution(A);
        if (xn != NULL)
        {
            printf("Infinite-solutions\n");
            printf("Particular-solution:\n");
            printMatrix(xp);
            printf("Null-space:\n");
            printf("%d %d\n", xn->rows, xn->cols);
            printMatrix(xn);
            continue;
        }
        printf("Unique-solution\n");
        printf("Particular-solution:\n");
        printMatrix(xp);
        printf("\n");
        free(A);
        free(b);
        free(xp);
        free(xn);
    }
    return 0;
}
