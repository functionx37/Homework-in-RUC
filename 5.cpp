#include<bits/stdc++.h>

int main(){
    char a;
    std::cin>>a;
    if(a>64&&a<91) a += 32;
    else a -= 32;
    std::cout<<a;
    return 0;
}