#include <bits/stdc++.h>

int main()
{
    int n,m;
    int sum=0;
    int a[105][105];
    std::cin>>n>>m;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            std::cin>>a[i][j];
            if(i==1||i==n||j==1||j==m){
            sum+=a[i][j];
            }
        }
    }
    std::cout<<sum;
    return 0;
}