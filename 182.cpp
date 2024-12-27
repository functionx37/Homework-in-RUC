#include<bits/stdc++.h>

int x[10005];

int main(){
    int n;
    std::cin>>n;
    char lu;
    int flag=0;
    for(int i=0;i<8;i++){
        int x=0;
        if(i==5||i==7) x++;
        if(i==1) x++;
        if(i==6) x++;
        if(i!=1) x++;
        if(i!=5&&i!=7) x++;
        if(i!=5&&i!=7) x++;
        if(i!=2) x++;
        if(i==5||i==7) x++;
        if(x==n){
            flag++;
            lu='A'+i;
        }
    }
    if(flag==1){
        std::cout<<lu;
    }
    else{
        printf("DONTKNOW");
    }
    return 0;
}    