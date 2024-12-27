#include<bits/stdc++.h>

int x[10005];

int main(){
    int n,a,b;
    x[1]=1;
    x[2]=1;
    std::cin>>a>>b>>n;
    for(int i=3;i<=n;i++){
        x[i]=(a*x[i-1]+b*x[i-2])%7;
    }
    std::cout<<x[n];
    return 0;
}    