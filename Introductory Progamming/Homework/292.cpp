#include<bits/stdc++.h>

double f(double x){
    if(x>1||x<-1) return 1/(1+x*x);
    return std::fabs(x-1)-2;
}

int main(){
    double a,x;
    std::cin>>a;
    x = f(f(a));
    printf("%.2lf",x);
    return 0;
}