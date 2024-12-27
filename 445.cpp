#include<bits/stdc++.h>

int main(){
    int m,n;
    scanf("%d,%d",&m,&n);
    int sum = 0;
    for(int i=m+1;i<n;i++){
        if(i % 2 == 1){
            sum += i;
        }
    }
    std::cout<<sum;
    return 0;
}