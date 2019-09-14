#include <stdio.h>
#include "matrix.c"

_Bool checkParticularSolution(matrix *A, matrix *xp, matrix *b);
_Bool checkNullspaceSolution(matrix *A, matrix *xn);

int main()
{
    FILE *fin, *fout, *fp;
    int t, m, n, mn, nn;
    char solution[100];
    int i;
    matrix *A, *xn, *xp, *b;
    fin = fopen("intinput.txt", "r");
    fout = fopen("intoutput.txt", "r");
    fp = fopen("results.txt", "w");
    fscanf(fin, "%d", &t);
    printf("%d\n", t);
    for (i = 0; i < t; i++)
    {
        printf("Test %d: \n", i + 1);
        fprintf(fp, "Test %d: \n", i + 1);
        fscanf(fin, "%d %d", &m, &n);
        printf("%d %d\n", m, n);
        A = newMatrix(m, n);
        readMatrix(A, fin);
        b = newMatrix(m, 1);
        readMatrix(b, fin);
        fscanf(fout, "%s", solution);
        printf("%s", solution);
        if (strcmp(solution, "No-solution") == 0)
        {
            fprintf(fp, "OK: No solution\n");
            continue;
        }
        xp = newMatrix(n, 1);
        readMatrix(xp, fout);
        printMatrix(xp);
        if (checkParticularSolution(A, xp, b))
        {
            printf("Particular solution matches\n");
            fprintf(fp, "OK: xp\n");
        }
        else
        {
            printf("Particular solution doesn't match\n");
            fprintf(fp, "ERROR: xp doesn't match\n");
        }
        if (!strcmp(solution, "Unique"))
        {
            printf("Unique solution detected\n");
            continue;
        }
        printf("Infinite solution detected\n");
        fscanf(fout, "%d %d", &mn, &nn);
        printf("%d %d\n", mn, nn);
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
        printMatrix(xn);
    }
    fclose(fin);
    fclose(fout);
    fclose(fp);
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
