#include <bits/stdc++.h>

int total=0;

void hua(int n,int k,int ans){
    if(k==0){
        if(n==0){total++;}
        return;
    }
    if(n<ans){
        return;
    }
    for(int i=ans;i<=n;i++){
        hua(n-i,k-1,i);
    }
}

int main()
{
    int n,k;
    std::cin>>n>>k;
    hua(n,k,1);
    std::cout<<total;
    return 0;
}