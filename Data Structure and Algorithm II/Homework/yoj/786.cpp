#include <stdio.h>

int main()
{
    int N, P, M;
    scanf("%d %d %d", &N, &P, &M);
    int A[100][100], B[100][100], C[100][100];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < P; j++)
        {
            scanf("%d", &A[i][j]);
        }
    }
    for (int i = 0; i < P; i++)
    {
        for (int j = 0; j < M; j++)
        {
            scanf("%d", &B[i][j]);
        }
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < P; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            printf("%d", C[i][j]);
            if (j < M - 1)
            {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}