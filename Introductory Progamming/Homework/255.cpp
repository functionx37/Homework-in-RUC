#include <bits/stdc++.h>

int main()
{
    int n,m;
    int a[25][25];
    int b[25][25];
    std::cin>>n>>m;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            std::cin>>a[i][j];
        }
    }
    for(int i=1;i<=m;i++){
        for(int j=1;j<=n;j++){
            b[i][j]=a[j][i];
        }
    }
    for(int i=1;i<=m;i++){
        for(int j=1;j<=n;j++){
            std::cout<<b[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    return 0;
}