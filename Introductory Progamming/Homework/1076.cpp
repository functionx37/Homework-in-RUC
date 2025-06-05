#include<bits/stdc++.h>

int main()
{
    int a;
    std::cin>>a;
    a--;
    std::cout<<a/12+1<<"-";
    a%=12;
    std::cout<<a/2+1;
    a%=6;
    std::cout<<"0"<<a%2+1;
    return 0;
}