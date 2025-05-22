#include<bits/stdc++.h>

int main(){
    int n; std::cin>>n;
    int a=0;
    if(n==0) std::cout<<"0,1";
    else{
        while(n>0){
            std::cout<< n%10 <<",";
            n /= 10;
            a++;
        }
        std::cout<<a;
    }
    return 0;
}