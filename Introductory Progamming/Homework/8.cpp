#include<bits/stdc++.h>

int main(){
    int a;
    std::cin>>a;
    if(a<60) printf("0");
    else if(a<63) printf("1.0");
    else if(a<66) printf("1.3");
    else if(a<70) printf("1.7");
    else if(a<73) printf("2.0");
    else if(a<76) printf("2.3");
    else if(a<80) printf("2.7");
    else if(a<83) printf("3.0");
    else if(a<86) printf("3.3");
    else if(a<90) printf("3.7");
    else printf("4.0");
    return 0;
}