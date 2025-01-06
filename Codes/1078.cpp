#include <bits/stdc++.h>

int n, m, tot = 0;
int num[10] = {0};

int nn;
int jie(int k){
    int x=1;
    while(k){
        x *= k;
        k--;
    }
    return x;
}

int a2i(int x,int y){
    int p = 1, ans =0;
    for(int i=y;i>x;i--){
        ans += num[i]*p;
        p *= 10;
    }
    return ans;
}

int main()
{
    std::cin >> n >> m;
    for (int i = 1; i <= m; i++)
    {
        num[i] = i;
    }
    nn = jie(m);
    for(int i=0;i<nn;i++){
        std::next_permutation(num+1,num+m+1);
        for(int s1 = 1; s1 < m - 1; s1++){
            for(int s2 = s1 + 1; s2<m ; s2++){
                int a = a2i(0,s1);
                int b = a2i(s1,s2);
                int c= a2i(s2,m);
                if(b%c==0&&a+b/c==n){
                    tot ++;
                }
            }
        }
    }
    std::cout << tot;
    return 0;
}