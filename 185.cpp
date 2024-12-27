#include<bits/stdc++.h>
double a[10005];
int main(){
    double n;
    std::cin>>n;
    a[0]=n/2.0;
    for(int i=1;;i++){
        a[i]=(a[i-1]+n/a[i-1])/2;
        if(fabs(a[i]-a[i-1])<1e-5){
            printf("%.6lf\n%d",a[i],i);
            break;
        }
    }
    return 0;
}    