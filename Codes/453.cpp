#include<bits/stdc++.h>

int main(){
    int a[205];
    int n;
    std::cin>>n;
    for(int i=0;i<n;i++){
        std::cin>>a[i];
    }
    std::sort(a,a+n);
    for(int i=0;i<n;i++){
        std::cout<<a[i]<<" ";
    }
    return 0;
}