#include<bits/stdc++.h>

int main(){
    int n,m=0;
    std::cin>>n;
    for(int a=0;a<=n;a++){
        for(int c=0;c<=n;c+=3)
            {
                int b=n-a-c;
                if(b>=0&&5*a+3*b+c/3==n){
                    m++;
                    printf("%d %d %d\n",a,b,c);
                }
            
        }
    }   
    std::cout<<m; 
    return 0;
}    