#include<bits/stdc++.h>

int wei(int i){
    if(i == 0) return 1;
    int n = 0;
    while(i>0){
        i /= 10;
        n++;
    }
    return n;
}

int sum = 0 ;

int main(){
    int a,b;
    std::cin>>a>>b;
    for(int i=a;i<=b;i++){
        int s = i*i - i;
        bool f = 1;
        int n = wei(i);
        while(n--){
            if(s%10!=0){
                f=0;
                break;
            }
            s /= 10;
        }
        if(f==1) sum += i;
    }
    std::cout<<sum;
    return 0;
}