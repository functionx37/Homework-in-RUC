#include <bits/stdc++.h>

int main()
{
    int n,m;
    int a=0,b=0,c=0,d=0;
    std::cin>>n>>m;
    for(int i=1;i<=n;i++){
        if(i%m==0){
            if(i%4==1) a++;
            if(i%4==2) b++;
            if(i%4==3) c++;
            if(i%4==0) d++;
        }
    }
    printf("%d %d %d %d",a,b,c,d);
    return 0;
}