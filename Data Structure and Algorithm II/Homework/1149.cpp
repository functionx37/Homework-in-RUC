#include <iostream>
using namespace std;

int sticks[] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

int calc(int x)
{
    if (x == 0)
        return 6;
    int sum = 0;
    while (x > 0)
    {
        sum += sticks[x % 10];
        x /= 10;
    }
    return sum;
}

int main()
{
    int n, cnt = 0;
    cin >> n;

    // 枚举个位数A和B
    for (int A = 0; A <= 1000; A++)
    {
        for (int B = 0; B <= 1000 && B <= A; B++)
        {
            int C = A + B;
            int total = calc(A) + calc(B) + calc(C) + 4;
            if (total == n)
            {
                // cout << A << " " << B << " " << C << endl;
                cnt += (A != B) ? 2 : 1;
            }
        }
    }
    cout << cnt;
    return 0;
}