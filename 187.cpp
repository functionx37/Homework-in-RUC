#include<bits/stdc++.h>

int gcd(int a,int b){
    if(a<b) return gcd(b,a);
    if(b==0) return a;
    return gcd(b,a%b);
}

int main(){
    int a,b;
    std::cin>>a>>b;
    std::cout<<a*b/gcd(a,b);
    return 0;
}