#include<bits/stdc++.h>

int main()
{   
	int n,m,ans,total=1;
    std::cin>>n>>m;
    for(int i=0;i<n;i++){
        int a;
        std::cin>>a;
        ans+=a;
        if(ans>m){
            total++;
            ans=a;
        }
    }
    std::cout<<total;
    return 0;  
}  