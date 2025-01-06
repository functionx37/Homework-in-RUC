#include<bits/stdc++.h>

bool a[1000000];

int main(){
    int n,m;
    std::cin>>n>>m;
    for(int i=0;i<n;i++){
        a[i] = 1;
    }
    int sum = 0;
    int x = n;
    int i = 0;
    while(x>1){
        sum += a[i];
        if(sum==m){
            a[i] = 0;
            x--;
            sum = 0;
        }
        i = (i+1)%n;
    }
    for(int i=0;i<n;i++){
        if(a[i]) std::cout<<(i+1);
    }
    return 0;
}