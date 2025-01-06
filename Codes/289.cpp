#include <bits/stdc++.h>

int cal(int a,int b,int c,int d,int e,int x){
    return a*x*x*x*x+b*x*x*x+c*x*x+d*x+e;
}

int main()
{
    int n,k;
    int s1=0,s2=0;
    int c1=0,c2=0;
    std::cin>>n>>k;
    for(int i=0;i<n;i++){
        int a1,a2;
        int a, b, c, d, e, x;
        std::cin>>a>>b>>c>>d>>e>>x>>a1>>a2;
        if(a1==cal(a,b,c,d,e,x)){
            s1++;
            c1++;
            if(c1>k){
                s1+=c1-k;
            }
        }
        else{
            c1=0;
        }
        if(a2==cal(a,b,c,d,e,x)){
            s2++;
            c2++;
            if(c2>k){
                s2+=c2-k;
            }
        }
        else{if(a2==cal(a,b,c,d,e,x)){
            s2++;
            c2++;
            if(c2>k){
                s2+=c2-k;
            }
        }
        else{
            c2=0;
        }
    }
    printf("%d %d",s1,s2);
    return 0;
}