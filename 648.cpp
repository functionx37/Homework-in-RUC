#include <bits/stdc++.h>

int n,x,y,z;
unsigned long long int a[1005];
int main()
{
    std::cin>>n>>x>>y>>z;
    for(int i=0;i<n;i++){
        std::cin>>a[i];
    }
    std::sort(a,a+n);
    for(int i=n-1;i>=n-x;i--){
        std::cout<<a[i]<<" ";
    }
    std::cout<<std::endl;
    for(int i=n-x-1;i>=n-y;i--){
        std::cout<<a[i]<<" ";
    }
    std::cout<<std::endl;
    for(int i=n-z;i<=n-y-1;i++){
        std::cout<<a[i]<<" ";
    }
    return 0;
}