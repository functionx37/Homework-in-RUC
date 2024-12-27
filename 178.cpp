#include<bits/stdc++.h>>

bool prime(int i){
    if(i==2||i==3){
        return 1;
    }
    if(i%2==0||i==1){
        return 0;
    }
    for(int k=3;k<=std::sqrt(i);k+=2){
        if(i%k==0){
            return 0;
        }
    }
    return 1;
}

int main(){
    int n;
    std::cin>>n;
    for(int i=0;i<n;i++){
        int a;
        std::cin>>a;
        if(prime(a)){
            std::cout<<"YES ";
        }
        else{
            std::cout<<"NO ";
        }
    }
    return 0;
}    