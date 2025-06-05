#include <bits/stdc++.h>

int main()
{
    int a[105];
    a[0]=0;
    for(int i=1;i<=100;i++){
        a[i]=i;
        if(i%4==0||i%4==3) a[i]=-a[i];
    }
    int m,n,ans=0;
    std::cin>>m>>n;
    for(int i=m;i<=n;i++){
        ans+=a[i];
    }
    std::cout<<ans;
    return 0;
}