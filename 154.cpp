#include<bits/stdc++.h>

int main(){
    double x;
    std::cin>>x;
    if(x<0) x *= -1;
    else if(x>1) x = x*x;
    else x= std::sqrt(x);
    std::printf("%.2lf",x);
    return 0;
}