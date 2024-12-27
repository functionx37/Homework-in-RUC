#include<bits/stdc++.h>

int main()
{   
	int maxx=0,minx=1000000,sum=0;
    int n;
    double ave;
    std::cin>>n;
    for(int i=0;i<n;i++){
        int a;
        std::cin>>a;
        maxx=std::max(a,maxx);
        minx=std::min(a,minx);
        sum+=a;
    }
    ave=sum*1.0/n;
    printf("%d %d %.0lf",maxx,minx,round(ave));
    return 0;  
}  