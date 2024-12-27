#include <bits/stdc++.h>

int mtr,n;
bool stop[100000]={0};
int sum = 0;

int main(){
    std::cin>>mtr>>n;
    for(int i=0;i<n;i++){
        int a;
        std::cin>>a;
        for(int j=a;j<a+mtr;j++){
            stop[j]=1;
        }
    }
    for(int i=0;i<100000;i++){
        sum+=stop[i];
    }
    std::cout<<sum;
    return 0;
}