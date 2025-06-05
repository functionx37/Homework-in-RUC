#include <bits/stdc++.h>

int cun[15][10];
int a1, a2, b1, b2, c1, c2, d1, d2;
int num1[10];
int num2[10];
int flag = 0;
int end;

int main()
{
    int n;
    std::cin >> n;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            char c;
            std::cin >> c;
            cun[i][j] = c - '0';
        }
    }
    for (int k = 1000; k <= 9999; k++)
    {
        int m = 0;
        d1 = k;
        a1 = d1 / 1000;
        d1 %= 1000;
        b1 = d1 / 100;
        d1 %= 100;
        c1 = d1 / 10;
        d1 %= 10;
        for (int o = 0; o < 10; o++)
        {
            num1[o] = 0;
        }
        num1[a1]++;
        num1[b1]++;
        num1[c1]++;
        num1[d1]++;
        for (int j = 0; j < n; j++)
        {
            int a2 = cun[j][0];
            int b2 = cun[j][1];
            int c2 = cun[j][2];
            int d2 = cun[j][3];
            int q = 0;
            if (a1 == a2)
                q++;
            if (b1 == b2)
                q++;
            if (c1 == c2)
                q++;
            if (d1 == d2)
                q++;
            int p = 0;
            for (int o = 0; o < 10; o++)
            {
                num2[o] = 0;
            }
            num2[a2]++;
            num2[b2]++;
            num2[c2]++;
            num2[d2]++;
            for (int d = 0; d < 10; d++)
            {
                if (num1[d] && num2[d])
                {
                    p += std::min(num1[d], num2[d]);
                }
            }
            if (p == cun[j][4] && q == cun[j][5])
            {
                m++;
            }
        }
        if (m == n)
        {
            flag++;
            end = k;
        }
    }
    if (flag != 1)
        printf("Not sure");
    else
        printf("%d", end);
    return 0;
}