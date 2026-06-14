#include <stdio.h>
#include <cstring>

int lcs[1005][1005] = {0};
char a[1005], b[1005];

int main()
{
    int i, j;
    scanf("%s", a);
    scanf("%s", b);
    for (i = 1; i <= strlen(a); i++)
    {
        for (j = 1; j <= strlen(b); j++)
        {
            if (a[i - 1] == b[j - 1])
            {
                lcs[i][j] = lcs[i - 1][j - 1] + 1;
            }
            else
            {
                if (lcs[i - 1][j] < lcs[i][j - 1])
                    lcs[i][j] = lcs[i][j - 1];
                else
                    lcs[i][j] = lcs[i - 1][j];
            }
        }
    }
    printf("%d", lcs[i - 1][j - 1]);
    return 0;
}