#include <bits/stdc++.h>

struct bus
{
    char b;
    char c;
    bool f=0;
} x[55];

int m[105][3]={0};

int main()
{
    int n;
    int f1 = 0, f2 = 0;
    std::cin >> n;
    int k = 0;
    while (1)
    {
        int a;
        std::cin >> a;
        if (a == 0)
            break;
        if (a == 1)
        {
            std::cin >> x[k].b >> x[k].c;
            x[k].f=1;
            k++;
        }
        if (a == 2)
        {
            char p, q;
            std::cin >> p >> q;
            for (int i = 0; i < k; i++)
            {
                if (x[i].b == p && x[i].c == q && x[i].f != 0)
                {
                    x[i].f = 0;
                    break;
                }
            }
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<3;j++){
            m[i][j]=1;
        }
    }
    for(int i=0;i<k;i++){
        if(x[i].f==1){
            for(int j=0;j<n;j++){
                if(x[i].b=='A'&&x[i].c=='B'&&m[j][0]==1){
                    m[j][0]=0;
                    break;
                }
                if(x[i].b=='B'&&x[i].c=='C'&&m[j][1]==1){
                    m[j][1]=0;
                    break;
                }
                if(x[i].b=='C'&&x[i].c=='D'&&m[j][2]==1){
                    m[j][2]=0;
                    break;
                }
                if(x[i].b=='A'&&x[i].c=='C'&&m[j][1]==1&&m[j][2]==1){
                    m[j][0]=0;
                    m[j][1]=0;
                    break;
                }
                if(x[i].b=='B'&&x[i].c=='D'&&m[j][2]==1&&m[j][1]==1){
                    m[j][2]=0;
                    m[j][1]=0;
                    break;
                }
                if(x[i].b=='A'&&x[i].c=='D'&&m[j][0]==1&&m[j][1]==1&&m[j][2]==1){
                    m[j][0]=0;
                    m[j][1]=0;
                    m[j][2]=0;
                    break;
                }
            }
        }
    }
    int sum=0;
    for(int j=0;j<n;j++){
        if(m[j][0]==1&&m[j][1]==1&&m[j][2]==1){
            sum++;
        }
    }
    std::cout<<sum;
    return 0;
}