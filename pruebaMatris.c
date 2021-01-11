#include <stdio.h>
#include <stdlib.h>
int main()
{
    int **a;
    int r = 4, c = 4, i, j;

    printf("\n\nEjemplo Matrices dinamicas\n\n");
    a = (int **)malloc(sizeof(int *) * r);
    for (i = 0; i < r; i++)
    {
        a[i] = (int *)malloc(sizeof(int) * c);
    }
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            a[i][j] = rand() % 10;
            printf("[%d] ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
    return 0;
}