#include <stdio.h>
#include "matrix.h"

_Bool checkParticularSolution(matrix *A, matrix *xp, matrix *b);
_Bool checkNullspaceSolution(matrix *A, matrix *xn);

int main()
{
/* The tester works only if INTERACTIVE is not defined and FILE_IN is. 
    This is because INTERACTIVE print statements are also printed in the output */
#ifdef FILE_IN
    FILE *fin, *fout, *fp;
    int t, m, n, mn, nn;
    char solution[100], dummy[100];
    int i, dummyI;
    matrix *A, *xn, *xp, *b;
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "r");
    fp = fopen("results.txt", "w");
    fscanf(fin, "%d", &t);
    for (i = 0; i < t; i++)
    {
        fprintf(fp, "Test %d: \n", i + 1);
        fscanf(fin, "%d %d", &m, &n);
        fscanf(fout, "%s %d", dummy, &dummyI);
        A = newMatrix(m, n);
        readMatrix(A, fin);
        b = newMatrix(m, 1);
        readMatrix(b, fin);
        fscanf(fout, "%s", solution);
        if (strcmp(solution, "No-solution") == 0)
        {
            fprintf(fp, "OK: No solution\n");
            continue;
        }
        fscanf(fout, "%s", dummy);
        xp = newMatrix(n, 1);
        readMatrix(xp, fout);
        if (checkParticularSolution(A, xp, b))
        {
            fprintf(fp, "OK: xp\n");
        }
        else
        {
            fprintf(fp, "ERROR: xp doesn't match\n");
        }
        if (!strcmp(solution, "Unique-solution"))
        {
            continue;
        }
        fscanf(fout, "%s", dummy);
        fscanf(fout, "%d %d", &mn, &nn);
        xn = newMatrix(mn, nn);
        readMatrix(xn, fout);
        if (checkNullspaceSolution(A, xn))
        {
            fprintf(fp, "OK: xn\n");
        }
        else
        {
            fprintf(fp, "ERROR: xn doesn't match\n");
        }
    }
    fclose(fin);
    fclose(fout);
    fclose(fp);
#endif
    return 0;
}

_Bool checkParticularSolution(matrix *A, matrix *xp, matrix *b)
{
    int i, j;
    double ans;
    _Bool flag = true;
    for (i = 0; i < A->rows; i++)
    {
        ans = 0;
        for (j = 0; j < A->cols; j++)
        {
            ans += ((A->data[i][j]) * (xp->data[j][0]));
        }
        flag = flag && isZero(ans - b->data[i][0], 1e-3);
    }
    return flag;
}

_Bool checkNullspaceSolution(matrix *A, matrix *xn)
{
    int i, j, k;
    double ans;
    _Bool flag = true;
    for (k = 0; k < xn->cols; k++)
    {
        for (i = 0; i < A->rows; i++)
        {
            ans = 0;
            for (j = 0; j < A->cols; j++)
            {
                ans += ((A->data[i][j]) * (xn->data[j][k]));
            }
            flag = flag && isZero(ans, 1e-3);
        }
    }
    return flag;
}
