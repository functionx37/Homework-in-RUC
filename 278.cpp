#include<bits/stdc++.h>

long long int a[35];

int main(){
    int n; std::cin>>n;
    a[0]=1; a[1]=1;
    for(int i=2;i<=n;i++){
        a[i] = a[i-1] + a[i-2];
    }
    std::cout<<a[n];
    return 0;
}