#include <bits/stdc++.h>

bool prime(int k){
    if(k==3) return 1;
    if(k%2==0) return 0;
    for(int i=3;i<=std::sqrt(k);i+=2){
        if(k%i==0) return 0;
    }
    return 1;
}

int main()
{
    int n;
    std::cin>>n;
    for(int i=3;i<=n/2;i+=2){
        if(prime(i)&&prime(n-i)){
            printf("%d=%d+%d",n,i,n-i);
            break;
        }
    }
    return 0;
}