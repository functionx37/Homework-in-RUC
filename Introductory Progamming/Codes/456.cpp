#include<bits/stdc++.h>

int main(){
    int n,m;
    std::cin>>n>>m;
    int mina = 2147483647;
    int maxa = -2147483644;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            int a;
            std::cin>>a;
            maxa = std::max(maxa,a);
        }
        mina = std::min(mina,maxa);
        maxa = -2147483644;
    }
    std::cout<<mina;
    return 0;
}