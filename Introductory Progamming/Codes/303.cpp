#include<bits/stdc++.h>

int main(){
    double a,b;
    std::cin>>a>>b;
    if(b==1) printf("%.2lf",a);
    if(b==2) printf("%.2lf",a*b*0.8);
    else if(b>2&&b<=5) printf("%.2lf",a*b*0.7);
    else if(b>5) printf("%.2lf",a*5*0.7+(b-5)*a*1.1);
    return 0;
}