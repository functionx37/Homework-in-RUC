#include <bits/stdc++.h>

int wei(int p){
    for(int i=0;i<10;i++){
        if(p>=std::pow(10,i)&&p<std::pow(10,i+1)){
            return i;
        }
    }
}

int rev(int k){
    int n = wei(k);
    int ans = 0;
    for(int i=0;i<=n;i++){
        ans += (k%10)*std::pow(10,n-i);
        k /= 10;
    }
    return ans;
}

int main(){
    int k;
    std::cin>>k;
    for(int i=1;i<=k;i++){
        if(i*i==rev(i*i)){
            printf("%d*%d=%d\n",i,i,i*i);
        }
    }
    return 0;
}