#include<bits/stdc++.h>

int main(){
    int a,b,n,k,m;
    std::cin>>a>>b>>n>>k>>m;
    int ans = 0;
    for(int i=a;i<=b;i++){
        if(i>=pow(10,m-1)&&i<pow(10,m)&&i%10==n&&i%k==0){
            ans++;
        }
    }
    std::cout<<ans;
    return 0;
}