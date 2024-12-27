#include<bits/stdc++.h>

int n,k;
int wei(){
    if(n==0) return 1;
    for(int i=0;i<15;i++){
        if(abs(n)>=std::pow(10,i)&&abs(n)<std::pow(10,i+1)){
            return i;
        }
    }
}

int main(){
    std::cin>>n;
    k=wei();
    int m=0;
    int p=1;
    if(n<0) {p=-1;n=-n;}
    for(int i=0;i<=k;i++){
        m+=(n%10)*pow(10,k-i);
        n/=10;
    }
    std::cout<<m*p;
    return 0;
}    