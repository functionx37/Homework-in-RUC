#include <bits/stdc++.h>

int main()
{
    int n,m,a,b;
    std::cin>>n>>m>>a>>b;
    for(int i=0;i<=1000&&n-i>=0;i++){
        if((i*a+(n-i)*b)==m){
            printf("%d %d",i,n-i);
            break;
        }
    }
    return 0;
}