/*
    Written as per ANSI C Standards. Can be verified by '-std=c89'
*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Macros */
#define _Bool int
#define true 1
#define false 0

/* Uncomment the below line for input from files */
#define FILE_IN 1

/* Uncomment the below line to have input prompts */
/* #define INTERACTIVE 1 */

/* Preprocessor directives for debugging */
/* #define DEBUG_MAIN 1 */
/* #define DEBUG_RREF 1 */
/* #define DEBUG_PS 1 */
/* #define DEBUG_NS 1 */

/* Functions for stack used in undoing column exchanges 
for finding null space */
#define push(sp, n) (*((sp)++) = (n))
#define pop(sp) (*--(sp))
#define STACKSIZE 1000

/* Encapsulating the matrix in a structure */
typedef struct
{
    int rows, cols;
    double **data;
} matrix;

/* Function declarations */
int min(int a, int b);
matrix *newMatrix(int rows, int cols);
void readMatrix(matrix *m, FILE *fp);
void printMatrix(matrix *m);
void copyMatrix(matrix *d, matrix *s);
matrix *addMatrix(matrix *m1, matrix *m2, _Bool opcode);
void switchRows(matrix *m, int row1, int row2);
void switchColumns(matrix *m, int col1, int col2);
void rref(matrix *A, matrix *b);
_Bool solutionExists(matrix *R, matrix *b);
matrix *particularSolution(matrix *R, matrix *b);
matrix *nullspaceSolution(matrix *R);
_Bool isZero(double a, double threshold);

int min(int a, int b)
{
    return (a < b) ? a : b;
}

matrix *newMatrix(int rows, int cols)
{
    int i;
    matrix *m = (matrix *)malloc(sizeof(matrix));
    m->rows = rows;
    m->cols = cols;
    m->data = (double **)calloc(rows, sizeof(double *));
    for (i = 0; i < rows; i++)
    {
        m->data[i] = (double *)calloc(cols, sizeof(double));
    }
    return m;
}

void readMatrix(matrix *m, FILE *fp)
{
    int row, col;
    for (row = 0; row < m->rows; row++)
    {
        for (col = 0; col < m->cols; col++)
        {
            fscanf(fp, "%lf", &(m->data[row][col]));
        }
    }
}

void printMatrix(matrix *m)
{
    int row, col;
    for (row = 0; row < m->rows; row++)
    {
        for (col = 0; col < m->cols; col++)
        {
            printf("%f ", m->data[row][col]);
        }
        /*        printf("%lf", m->data[row][m->cols - 1]); */
        printf("\n");
    }
}

void switchRows(matrix *m, int row1, int row2)
{
    double *arr = calloc(m->cols, sizeof(double));
    int i;
    if (row1 >= m->rows || row2 >= m->rows)
    {
        printf("Invalid row shift!\n");
        return;
    }
    for (i = 0; i < m->cols; i++)
    {
        arr[i] = m->data[row1][i];
    }
    for (i = 0; i < m->cols; i++)
    {
        m->data[row1][i] = m->data[row2][i];
    }
    for (i = 0; i < m->cols; i++)
    {
        m->data[row2][i] = arr[i];
    }
    free(arr);
}

void switchColumns(matrix *m, int col1, int col2)
{
    double *arr = calloc(m->rows, sizeof(double));
    int i;
    if (col1 == col2)
    {
        return;
    }
    if (col1 >= m->cols || col2 >= m->cols)
    {
        printf("Invalid column shift\n");
        return;
    }
    for (i = 0; i < m->rows; i++)
    {
        arr[i] = m->data[i][col1];
    }
    for (i = 0; i < m->rows; i++)
    {
        m->data[i][col1] = m->data[i][col2];
    }
    for (i = 0; i < m->rows; i++)
    {
        m->data[i][col2] = arr[i];
    }
    free(arr);
}

void rref(matrix *A, matrix *b)
{
    int pivotRow = 0;
    int pivotCol = 0;
    int j;
    int switchRow;
    double r;
    int k;
    double quotient;
    /* Converting to upper triangular */
    while (pivotRow < A->rows && pivotCol < A->cols)
    {
        switchRow = -1;
        if (isZero(A->data[pivotRow][pivotCol], 1e-3))
        {
#ifdef DEBUG_RREF
            printf("Zero at: %d %d\n", pivotRow, pivotCol);
#endif
            for (j = pivotRow + 1; j < A->rows; j++)
            {
                if (!isZero(A->data[j][pivotCol], 1e-3))
                {
                    switchRow = j;
#ifdef DEBUG_RREF
                    printf("Switch %d %d\n", pivotRow, j);
#endif
                    switchRows(A, pivotRow, switchRow);
                    switchRows(b, pivotRow, switchRow);
                    break;
                }
            }
            if (switchRow == -1)
            {
#ifdef DEBUG_RREF
                printf("No switch for row: %d\n", pivotRow);
#endif
                pivotCol++;
                continue;
            }
        }
        for (j = pivotRow + 1; j < A->rows; j++)
        {
            if (!isZero(A->data[j][pivotCol], 1e-3))
            {
                r = A->data[j][pivotCol] / A->data[pivotRow][pivotCol];
#ifdef DEBUG_RREF
                printf("%d %d %d %lf %lf r : %lf\n\n", pivotRow, pivotCol, j, A->data[j][pivotCol], A->data[pivotRow][pivotCol], r);
#endif
                for (k = pivotCol; k < A->cols; k++)
                {
                    A->data[j][k] -= r * A->data[pivotRow][k];
                }
                b->data[j][0] -= r * b->data[pivotRow][0];
            }
#ifdef DEBUG_RREF
            printMatrix(A);
            printf("\n");
#endif
        }
        pivotRow++;
        pivotCol++;
    }
#ifdef DEBUG_RREF
    printf("A converted to upper triangular: ");
    printMatrix(A);
    printMatrix(b);
#endif

    /*  Converting to rref from upper triangular
        Select a pivot */
    pivotRow = 0;
    pivotCol = 0;
    while (pivotRow < A->rows && pivotCol < A->cols)
    {
        if (isZero(A->data[pivotRow][pivotCol], 1e-3))
        {
            pivotCol++;
            continue;
        }
        else
        {
            for (j = pivotRow - 1; j >= 0; j--)
            {
                r = A->data[j][pivotCol] / A->data[pivotRow][pivotCol];
#ifdef DEBUG_RREF
                printMatrix(A);
                printf("\n");
                printf("%d %d %d %lf %lf r : %lf\n\n", pivotRow, pivotCol, j, A->data[j][pivotCol], A->data[pivotRow][pivotCol], r);
                // printf("%d %d %lf %lf r : %lf\n", , j, A->data[j][i], A->data[i][i], r);
#endif
                /* Row operations */
                for (k = pivotCol; k < A->cols; k++)
                {
                    A->data[j][k] -= r * A->data[pivotRow][k];
                }
                b->data[j][0] -= r * b->data[pivotRow][0];
            }
        }
        pivotRow++;
        pivotCol++;
    }
    /* Make all pivot elements '1' */
    pivotRow = 0;
    pivotCol = 0;
    while (pivotRow < A->rows && pivotCol < A->cols)
    {
        if (isZero(A->data[pivotRow][pivotCol], 1e-3))
        {
            pivotCol++;
            continue;
        }
        quotient = A->data[pivotRow][pivotCol];
        for (j = pivotCol; j < A->cols; j++)
        {
            if (A->data[pivotRow][j])
                A->data[pivotRow][j] /= quotient;
        }
        if (b->data[pivotRow][0])
            b->data[pivotRow][0] /= quotient;
        pivotRow++;
        pivotCol++;
    }
}

/* Check if there are any inconsistencies in Rx = b */
_Bool solutionExists(matrix *R, matrix *b)
{
    int i, j;
    _Bool zeroFlag;
    for (i = 0; i < R->rows; i++)
    {
        zeroFlag = true;
        for (j = 0; j < R->cols; j++)
        {
            if (!isZero(R->data[i][j], 1e-3))
            {
                zeroFlag = false;
                break;
            }
        }
        if (zeroFlag)
        {
            if (!isZero(b->data[i][0], 1e-3))
            {
                return false;
            }
        }
    }
    return true;
}

/* Deduce particular solution from reduced row echelon matrix with pivot elements 1 */
matrix *particularSolution(matrix *R, matrix *b)
{
    matrix *xp = newMatrix(R->cols, 1);
    int pivotRow = 0, pivotCol = 0;
    while (pivotRow < R->rows && pivotCol < R->cols)
    {
        if (!R->data[pivotRow][pivotCol])
        {
            pivotCol++;
            continue;
        }
        xp->data[pivotCol][0] = b->data[pivotRow][0] / R->data[pivotRow][pivotCol];
#ifdef DEBUG_PS
        printf("%d %d\n", pivotRow, pivotCol);
#endif
        pivotRow++;
        pivotCol++;
    }
#ifdef DEBUG_PS
    printf("Particular solution calculated\n");
#endif
    return xp;
}

_Bool isZero(double a, double threshold)
{
    if (a >= -threshold && a <= threshold)
    {
        return true;
    }
    return false;
}

matrix *nullspaceSolution(matrix *R)
{
#ifdef DEBUG_NS
    printf("Calculating null space solution\n");
#endif
    int maxPivots = min(R->rows, R->cols);
    int stackA[STACKSIZE], stackB[STACKSIZE];
    int *sA, *sB;
    int length = 0;
    int pivots = 0;
    int i, j, i1, j1;
    matrix *xn;
    int Frows, Fcols;
    sA = stackA;
    sB = stackB;
    /* Bring the pivots to the diagonal, if they are not */
    for (i = 0; i < maxPivots; i++)
    {
#ifdef DEBUG_NS
        printf("%lf\n", R->data[i][i]);
#endif
        if (isZero(R->data[i][i], 1e-3))
        {

            for (j = i + 1; j < R->cols; j++)
            {
                if (!isZero(R->data[i][j], 1e-3))
                {
                    /* If column exchange is required, push the indices in stacks, 
                        so that they can be undone */
                    push(sA, i);
                    push(sB, j);
                    length++;
#ifdef DEBUG_NS
                    printf("%d %d\n", i, j);
#endif
                    /* Switch appropriate columns */
                    switchColumns(R, i, j);
                    pivots++;
                    break;
                }
            }
        }
        else
        {
            pivots++;
        }
    }
#ifdef DEBUG_NS
    printf("%d", pivots);
    printf("Switched R: \n");
    printMatrix(R);
    printf("Max and Pivots: %d %d\n", maxPivots, pivots);
#endif
    if (R->cols == pivots)
    {
#ifdef DEBUG_NS
        printf("Returning NULL");
#endif
        return NULL;
    }

    /* Dimensions of the basis matrix of the null space can be determined now */
    xn = newMatrix(R->cols, R->cols - pivots);
    Frows = pivots, Fcols = R->cols - pivots;

    /* Copy '-F' to the new matrix */
    for (i = 0; i < Frows; i++)
    {
        for (j = 0; j < Fcols; j++)
        {
            xn->data[i][j] = -(R->data[i][pivots + j]);
        }
    }

    /* Fill the remainder space with 'I' */
    i1 = 0;
    for (i = Frows; i < xn->rows; i++, i1++)
    {
        j1 = 0;
        for (j = 0; j < xn->cols; j++, j1++)
        {
            if (i1 == j1)
                xn->data[i][j] = 1;
            else
                xn->data[i][j] = 0;
        }
    }

    /* Switch rows in xn in the reverse order of the column swaps done in A */
    while (length--)
    {
        switchRows(xn, pop(sA), pop(sB));
    }

    return xn;
}
