#include <bits/stdc++.h>

double jie(int k){
    if(k==0) return 1;
    double x=1;
    for(int i=1;i<=k;i++){
        x = x*i;
    }
    return x;
}

int main()
{
    double x,y,pi=3.1415926535897932384626433832795;
    std::cin>>x>>y;
    double sin=0,cos=0;
    while(x>2*pi){
        x-=2*pi;
    }
    for(int i=1;;i+=2){
        double a=std::pow(x,i)/jie(i);
        if(std::fabs(a)<y) break;
        if(i%4==3) a=0-a;
        sin+=a;
    }
    for(int i=0;;i+=2){
        double a=std::pow(x,i)/jie(i);
        if(std::fabs(a)<y) break;
        if(i%4==2) a=0-a;
        cos+=a;
    }
    printf("%lf\n%lf",sin,cos);
    return 0;
}