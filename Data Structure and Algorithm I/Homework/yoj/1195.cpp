#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;
    stack<int> st;
    for (int i = 0; i < n; i++)
    {
        int temp;
        cin >> temp;
        st.push(temp);
    }
    int ans = 0;
    while (st.size() >= 4)
    {
        int num[4];
        for (int i = 0; i < 4; i++)
        {
            num[i] = st.top();
            st.pop();
        }
        sort(num, num + 4);
        if (num[0] + num[3] == num[1] + num[2])
        {
            ans += 4;
            continue;
        }
        else if (num[0] + num[1] + num[2] == num[3])
        {
            ans += 4;
            continue;
        }
        else if (num[1] + num[2] == num[3])
        {
            ans += 3;
            st.push(num[0]);
            continue;
        }
        else if (num[0] + num[2] == num[3])
        {
            ans += 3;
            st.push(num[1]);
            continue;
        }
        else if (num[0] + num[1] == num[3])
        {
            ans += 3;
            st.push(num[2]);
            continue;
        }
        else if (num[0] + num[1] == num[2])
        {
            ans += 3;
            st.push(num[3]);
            continue;
        }
        else if (num[3] == num[2])
        {
            ans += 2;
            st.push(num[1]);
            st.push(num[0]);
            continue;
        }
        else if (num[1] == num[2])
        {
            ans += 2;
            st.push(num[3]);
            st.push(num[0]);
            continue;
        }
        else if (num[1] == num[0])
        {
            ans += 2;
            st.push(num[3]);
            st.push(num[2]);
            continue;
        }
        else
        {
            break;
        }
    }
    cout << ans;
    return 0;
}