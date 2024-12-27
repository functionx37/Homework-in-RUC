#include<bits/stdc++.h>

int main(){
    int n;
    std::cin>>n;
    for(int i=1;i<=n;i++){
        if(n%i==0) std::printf("%d ",i);
    }
    return 0;
}