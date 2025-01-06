#include<bits/stdc++.h>

int ack(int m,int n){
    if(m==0) return n+1;
    if(n==0) return ack(m - 1, 1);
    return ack(m - 1, ack(m, n - 1)) ;
}

int main(){
    int a,b;
    std::cin>>a>>b;
    std::cout<<ack(a,b);
    return 0;
}