#include <bits/stdc++.h>

int a[10005];

int main()
{
    int n,m;
    std::cin>>n>>m;
    for(int i=0;i<m;i++){
        std::cin>>a[i];
    }
    if(m==1){
        std::cout<<a[0];
        return 0;
    }
    std::sort(a,a+m);
    int ans=1;
    for(int i=1;i<m;i++){
        if(a[i]==a[i-1]){
            ans++;
            if(ans>m/2){
                std::cout<<a[i];
                return 0;
            }
        }
        else ans=1;
    }
    std::cout<<"-1";
    return 0;
}