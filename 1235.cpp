#include<bits/stdc++.h>

int n,m;
int way[35];

void go(int k){
    for(int i=1;i<=m&&k-i>=0;i++){
        way[k] += way[k-i];
    }
}

int main(){
    std::cin>>n>>m;
    way[0] = 1;
    for(int i=1;i<=n;i++){
        go(i);
    }
    std::cout<<way[n];
    return 0;
}