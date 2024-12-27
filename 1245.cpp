#include<bits/stdc++.h>

long long n;
bool ava(long long k){
    long long ans = 0, n1 = 0;
    while(k){
        ans += k%10;
        n1++;
        k /= 10;
    }
    if((1.0*ans/n1) > 4)
    return 1;
    return 0;
}
int tot = 0;
int main(){
    long long now, pre1 = 1, pre2=0;
    std::cin>>n;
    while(1){
        now = pre1 +pre2;
        if(now>=n) break;
        if(ava(now)){
            tot++;
        }
        pre2 =pre1;
        pre1 = now;
    }
    std::cout<<tot;
    return 0;
}