#include <bits/stdc++.h>

int main()
{
    int n;
    std::cin>>n;
    double h,l;
    char a,b;
    std::cin>>h>>a>>l>>b;
    if(a=='C') h= h*9/5 + 32;
    if(b=='C') l= l*9/5 + 32;
    double sum=0;
    int x=0,y=0;
    for(int i=0;i<n;i++){
        double p;
        char q;
        std::cin>>p>>q;
        if(q=='C') p= p*9/5 + 32;
        sum+=p;
        if(p>=h) x++;
        if(p<=l) y++;
    }
    sum/=n;
    printf("%.3lfF\n%d %d",sum,x,y);
    return 0;
}