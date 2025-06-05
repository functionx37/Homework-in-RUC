#include<bits/stdc++.h>

int main(){
    double a,b,c;
    std::cin>>a>>b>>c;
    double p = (a+b+c)/2;
    double s = std::sqrt(p*(p-a)*(p-b)*(p-c));
    printf("%.2lf",s);
    return 0;
}